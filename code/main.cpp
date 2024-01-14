#include "inc/cc_device_data_aggregator_handler.hpp"

int main()
{
    bool dataAggregatorSuccess = dataAggregatorHandler::getInstance()->dataAggregator_output();
    if(!dataAggregatorSuccess){
        cout<< "ERROR: Issue in aggregating data" << endl;
        cout << "Exiting process" << endl << endl;
        return 0;
    }

    return 0;
}
