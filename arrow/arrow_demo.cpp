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
    const std::vector<struct data_row>& rows) {
	
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
int main(int argc, char **argv) {
	std::vector<data_row> rows = {
     {1, 1, {10.0}}, {2, 3, {11.0, 12.0, 13.0}}, {3, 2, {15.0, 25.0}}};


	std::shared_ptr<arrow::Table> table;
	arrow::Result<std::shared_ptr<arrow::Table>> table_result = VectorToColumnarTable(rows);
	table = std::move(table_result).ValueOrDie();

	for (const std::string& colName : table->ColumnNames()) {
		std::cout << colName << std::endl;
	}

	std::cout << table->GetColumnByName("id")->ToString() << std::endl;

	int64_t first_id; 
	// arrow::Status status = table->GetColumnByName("id")->GetScalar(0).Value(&first_id);	
	// if (status.OK()) {
	// 	std::cout << "first_id: " << first_id << std::endl;
	// } else {
	// 	std::cout << "got first_id failed: " <<  status.CodeAsString() << std::endl;
	// }
	return 0;	
}