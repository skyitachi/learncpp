#include <arrow/api.h>
#include <arrow/result.h>
#include <vector>
#include <iostream>
#include <random>

using arrow::DoubleBuilder;
using arrow::Int64Builder;
using arrow::ListBuilder;

struct data_row {
	int64_t id;
	int64_t components;
	std::vector<double> component_cost;
};

arrow::Result<std::shared_ptr<arrow::Table>> VectorToColumnarTable(
    const std::vector<data_row>& rows) {
	
	arrow::MemoryPool* pool = arrow::default_memory_pool();
	Int64Builder id_builder(pool);
	Int64Builder component_builder(pool);
	ListBuilder component_cost_builder(pool, std::make_shared<DoubleBuilder>(pool));
	DoubleBuilder* component_item_cost_builder =
      (static_cast<DoubleBuilder*>(component_cost_builder.value_builder()));

	for (const data_row& row: rows) {
		ARROW_RETURN_NOT_OK(id_builder.Append(row.id));
		ARROW_RETURN_NOT_OK(component_builder.Append(row.components));
		ARROW_RETURN_NOT_OK(component_cost_builder.Append());

		ARROW_RETURN_NOT_OK(component_item_cost_builder->AppendValues(row.component_cost));
	}

	std::shared_ptr<arrow::Array> id_array;
	ARROW_RETURN_NOT_OK(id_builder.Finish(&id_array));
	std::shared_ptr<arrow::Array> components_array;
	ARROW_RETURN_NOT_OK(component_builder.Finish(&components_array));
	std::shared_ptr<arrow::Array> component_cost_array;
	ARROW_RETURN_NOT_OK(component_cost_builder.Finish(&component_cost_array));

	std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
	arrow::field("id", arrow::int64()), arrow::field("components", arrow::int64()),
	arrow::field("component_cost", arrow::list(arrow::float64()))};

 	auto schema = std::make_shared<arrow::Schema>(schema_vector);

	std::shared_ptr<arrow::Table> table =
    arrow::Table::Make(schema, {id_array, components_array, component_cost_array});

  	return table;
}


void array_demo() {
  arrow::Int64Builder builder;
  builder.Append(1);
  builder.Append(2);
  builder.Append(3);
  builder.AppendNull();
  builder.Append(5);
  builder.Append(6);
  builder.Append(7);
  builder.Append(8);

  auto maybe_array = builder.Finish();
  if (!maybe_array.ok()) {
    std::cout << "allocate failed" << std::endl;
  }

  std::shared_ptr<arrow::Array> array = *maybe_array;
  auto int64_array = std::static_pointer_cast<arrow::Int64Array>(array);
  const uint8_t* nullbitmap = int64_array->null_bitmap_data();

  const int64_t* data = int64_array->raw_values();
  int64_t index = 2;

  for (int64_t i = 0; i < 8; i++) {
    if (!int64_array->IsNull(i)) {
      int64_t value = int64_array->Value(i);
      std::cout << "index " << i << ", value: " << value << std::endl;
    } else {
      std::cout << "index " << i << " is null" << std::endl;
    }
  }
  printf("nullmap value %d\n", *nullbitmap);
}

// 一般使用内置api完成创建嵌套list
arrow::Status arrow_nested_list_demo() {
  arrow::MemoryPool* pool = arrow::default_memory_pool();

  // 创建一个ListBuilder对象，用于构建一个整数列表
  auto inner_list_builder =
      std::make_shared<arrow::ListBuilder>(pool, std::make_shared<arrow::Int32Builder>(pool));

  auto *inner_list_value_builder = static_cast<arrow::Int32Builder*>(inner_list_builder->value_builder());
  inner_list_builder->Append();
  inner_list_value_builder->Append(10);
  inner_list_value_builder->Append(20);
  inner_list_builder->Append(false);


  auto list_builder =
      std::make_shared<arrow::ListBuilder>(pool, inner_list_builder);

  auto* list_value_build = static_cast<arrow::ListBuilder*>(list_builder->value_builder());

  list_builder->Append();
//  list_builder->AppendValues(inner_list_builder->Finish());

  // 如何append

//  list_value_build->AppendArraySlice(arr, 0, 2);

  auto offset_builder = std::make_shared<arrow::Int32Builder>(pool);
  int32_t rows  = 7;
  offset_builder->Append(0);
  offset_builder->Append(2);
  offset_builder->Append(4);
  offset_builder->Append( rows);

  auto value_builder = std::make_shared<arrow::DoubleBuilder>();
  for(int i = 0; i < rows; i++) {
    value_builder->Append(i);
  }
  ARROW_ASSIGN_OR_RAISE(auto offsets, offset_builder->Finish());
  ARROW_ASSIGN_OR_RAISE(auto values, value_builder->Finish());

  ARROW_ASSIGN_OR_RAISE(auto array,
                        arrow::ListArray::FromArrays(*offsets.get(), *values.get()));


  auto outer_offsets_builder = std::make_shared<arrow::Int32Builder>(pool);
  outer_offsets_builder->Append(0);
  outer_offsets_builder->Append(2);
  outer_offsets_builder->Append(3);

  ARROW_ASSIGN_OR_RAISE(auto outer_offsets, outer_offsets_builder->Finish());

  ARROW_ASSIGN_OR_RAISE(auto nested_array,
                        arrow::ListArray::FromArrays(*outer_offsets.get(), *array.get()));



  std::cout << array->ToString()  << std::endl;

  std::cout << nested_array->ToString() << std::endl;



  return arrow::Status::OK();


}

class RandomBatchGenerator {
public:
  std::shared_ptr<arrow::Schema> schema;

  RandomBatchGenerator(std::shared_ptr<arrow::Schema> schema) : schema(schema){};

  arrow::Result<std::shared_ptr<arrow::RecordBatch>> Generate(int32_t num_rows) {
    num_rows_ = num_rows;
    for (std::shared_ptr<arrow::Field> field : schema->fields()) {
      ARROW_RETURN_NOT_OK(arrow::VisitTypeInline(*field->type(), this));
    }

    return arrow::RecordBatch::Make(schema, num_rows, arrays_);
  }

  // Default implementation
  arrow::Status Visit(const arrow::DataType& type) {
    return arrow::Status::NotImplemented("Generating data for", type.ToString());
  }

  arrow::Status Visit(const arrow::DoubleType&) {
    auto builder = arrow::DoubleBuilder();
    std::normal_distribution<> d{/*mean=*/5.0, /*stddev=*/2.0};
    for (int32_t i = 0; i < num_rows_; ++i) {
      ARROW_RETURN_NOT_OK(builder.Append(d(gen_)));
    }
    ARROW_ASSIGN_OR_RAISE(auto array, builder.Finish());
    arrays_.push_back(array);
    return arrow::Status::OK();
  }

  arrow::Status Visit(const arrow::ListType& type) {
    // Generate offsets first, which determines number of values in sub-array
    std::poisson_distribution<> d{/*mean=*/4};
    auto builder = arrow::Int32Builder();
    ARROW_RETURN_NOT_OK(builder.Append(0));
    int32_t last_val = 0;
    for (int32_t i = 0; i < num_rows_; ++i) {
      last_val += d(gen_);
      ARROW_RETURN_NOT_OK(builder.Append(last_val));
    }
    ARROW_ASSIGN_OR_RAISE(auto offsets, builder.Finish());

    // Since children of list has a new length, will use a new generator
    RandomBatchGenerator value_gen(arrow::schema({arrow::field("x", type.value_type())}));
    // Last index from the offsets array becomes the length of the sub-array
    ARROW_ASSIGN_OR_RAISE(auto inner_batch, value_gen.Generate(last_val));
    std::shared_ptr<arrow::Array> values = inner_batch->column(0);

    ARROW_ASSIGN_OR_RAISE(auto array,
                          arrow::ListArray::FromArrays(*offsets.get(), *values.get()));
    arrays_.push_back(array);

    return arrow::Status::OK();
  }

protected:
  std::random_device rd_{};
  std::mt19937 gen_{rd_()};
  std::vector<std::shared_ptr<arrow::Array>> arrays_;
  int32_t num_rows_;
};  // RandomBatchGenerator

arrow::Status create_arrays_with_schema_demo() {
  std::shared_ptr<arrow::Schema> schema =
      arrow::schema({arrow::field("x", arrow::float64()),
                     arrow::field("y", arrow::list(arrow::float64()))});

  RandomBatchGenerator generator(schema);
  ARROW_ASSIGN_OR_RAISE(std::shared_ptr<arrow::RecordBatch> batch, generator.Generate(5));

  std::cout << "Created batch: \n" << batch->ToString();
// Consider using ValidateFull to check correctness
  ARROW_RETURN_NOT_OK(batch->ValidateFull());

  return arrow::Status::OK();
}

arrow::Status arrow_list_demo() {
//  std::shared_ptr<arrow::DataType> type;
//  type = arrow::int16();
//  type = arrow::list(arrow::float32());

  arrow::MemoryPool* pool = arrow::default_memory_pool();

  // 创建一个ListBuilder对象，用于构建一个整数列表
  auto list_builder =
      std::make_shared<arrow::ListBuilder>(pool, std::make_shared<arrow::Int32Builder>(pool));

  auto *list_value_builder = static_cast<arrow::Int32Builder*>(list_builder->value_builder());

  // Indicate the start of a new list row. This will memorise the current
  // offset in the values builder.
  ARROW_RETURN_NOT_OK(list_builder->Append());
  // 添加数据到列表中
  list_value_builder->Append(10);
  list_value_builder->Append(20);

  list_builder->Append(false);

  list_builder->Append();
  list_value_builder->Append(10);
  list_value_builder->Append(20);
  list_value_builder->Append(30);

  list_builder->Append();
  list_value_builder->Append(1);
  list_value_builder->Append(2);
  list_value_builder->Append(3);


  // 完成列表的构建，并创建一个ListArray对象
  std::shared_ptr<arrow::Array> list_array;
  list_builder->Finish(&list_array);

  // 输出列表中的数据
  auto list = std::static_pointer_cast<arrow::ListArray>(list_array);
  for (int i = 0; i < list->length(); i++) {
    if (list->IsNull(i)) {
      std::cout << "null" << std::endl;
    } else {
      auto value = std::static_pointer_cast<arrow::Int32Array>(list->value_slice(i));
      for (int j = 0; j < value->length(); j++) {
        std::cout << value->Value(j) << std::endl;
      }
    }
  }

  return arrow::Status::OK();
}

int main(int argc, char **argv) {
  arrow_nested_list_demo();
  create_arrays_with_schema_demo();
  arrow_list_demo();
  array_demo();
	std::vector<data_row> rows = {
     {1, 1, {10.0}}, {2, 3, {11.0, 12.0, 13.0}}, {3, 2, {15.0, 25.0}}};


	std::shared_ptr<arrow::Table> table;
	arrow::Result<std::shared_ptr<arrow::Table>> table_result = VectorToColumnarTable(rows);
	table = std::move(table_result).ValueOrDie();

	for (const std::string& colName : table->ColumnNames()) {
		std::cout << colName << std::endl;
	}

	std::cout << table->GetColumnByName("id")->ToString() << std::endl;

  std::shared_ptr<arrow::Scalar> first_id;
  auto status = table->GetColumnByName("id")->GetScalar(0).Value(&first_id);
  if (status.ok()) {
    auto real_value = static_cast<arrow::Int64Scalar&>(*first_id).value;
    std::cout << "first_id: " << real_value << std::endl;
  } else {
    std::cout << "got first_id failed: "  << status.CodeAsString() << std::endl;
  }
	return 0;	
}