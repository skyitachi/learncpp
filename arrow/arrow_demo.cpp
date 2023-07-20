#include <arrow/api.h>
#include <arrow/result.h>
#include <vector>
#include <iostream>

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