#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_bp.h"
#include "bp.cc"

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file
    
    gshared_bp bp_bimodal;
    gshared_bp bp_gshared;
    gshared_bp bp_hybrid;


    unsigned long int bimodal_total_counter = 0;
    unsigned long int gshared_total_counter = 0;
    unsigned long int hybrid_total_counter = 0;

    unsigned long int bimodal_miss_counter = 0;
    unsigned long int gshared_miss_counter = 0;
    unsigned long int hybrid_miss_counter = 0;

    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.bp_name  = argv[1];
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        if(argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M2       = strtoul(argv[2], NULL, 10);
        trace_file      = argv[3];
        printf("COMMAND\n%s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);
        bp_bimodal.config_predictor("bimodal", params.M2);
        bp_bimodal.prediction_table_init();
    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        if(argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M1       = strtoul(argv[2], NULL, 10);
        params.N        = strtoul(argv[3], NULL, 10);
        trace_file      = argv[4];
        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);
        bp_gshared.config_predictor("gshare", params.M1, params.N);
        bp_gshared.prediction_table_init();

    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if(argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.K        = strtoul(argv[2], NULL, 10);
        params.M1       = strtoul(argv[3], NULL, 10);
        params.N        = strtoul(argv[4], NULL, 10);
        params.M2       = strtoul(argv[5], NULL, 10);
        trace_file      = argv[6];
        printf("COMMAND\n%s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);

        bp_bimodal.config_predictor("bimodal", params.M2);
        bp_bimodal.prediction_table_init();

        bp_gshared.config_predictor("gshare", params.M1, params.N);
        bp_gshared.prediction_table_init();

        bp_hybrid.config_hybrid(params.K);
        bp_hybrid.selector_table_init();

    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }
    
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    char str[2];
    while(fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
        
        outcome = str[0];
        //if (outcome == 't')
        //    printf("%lx %s\n", addr, "t");           // Print and test if file is read correctly
        //else if (outcome == 'n')
        //    printf("%lx %s\n", addr, "n");          // Print and test if file is read correctly
        
        /*************************************
            Add branch predictor code here
        **************************************/
    if(strcmp(params.bp_name, "bimodal") == 0){

            bimodal_total_counter++;

            bp_bimodal.addr_to_identifier(addr);
            //std::cout << bp.get_prediction(params.bp_name) << std::endl;
            if(outcome != bp_bimodal.get_prediction("bimodal")){
                bimodal_miss_counter++;
            }
            bp_bimodal.update_outcome(outcome);
        }
    else if(strcmp(params.bp_name, "gshare") == 0){
            
            gshared_total_counter++;

            bp_gshared.addr_to_identifier(addr);
            if(outcome != bp_gshared.get_prediction("gshare")){
                gshared_miss_counter++;
            }
            bp_gshared.update_bh_register(outcome);
            bp_gshared.update_outcome(outcome);
        }
    else if(strcmp(params.bp_name, "hybrid") == 0){

            hybrid_total_counter++;

            bp_bimodal.addr_to_identifier(addr);
            char bimodal_prediction = bp_bimodal.get_prediction("bimodal");

            bp_gshared.addr_to_identifier(addr);
            char gshared_prediction = bp_gshared.get_prediction("gshare");

            bp_hybrid.get_selector_index(addr);
            int selector_value = bp_hybrid.get_selector_value();

            bp_gshared.update_bh_register(outcome);

            if(selector_value >= 2){
                bp_gshared.update_outcome(outcome);
                if(outcome != gshared_prediction)
                hybrid_miss_counter++;
            }
            else{
                bp_bimodal.update_outcome(outcome);
                if(outcome != bimodal_prediction)
                hybrid_miss_counter++;
            }

            bp_hybrid.update_selector(outcome, gshared_prediction, bimodal_prediction);
        }
    }

// DISPLAY
    std::cout << "OUTPUT" << std::endl;

    if(strcmp(params.bp_name, "bimodal") == 0){

        std::cout << "number of predictions: " << bimodal_total_counter << std::endl;
        std::cout << "number of mispredictions: " << bimodal_miss_counter << std::endl;
        double result = (static_cast<double>(bimodal_miss_counter)/bimodal_total_counter)*100;
        printf("misprediction rate: %.2lf%%\n", result);

        std::cout << "FINAL BIMODAL CONTENTS" << std::endl;
        bp_bimodal.display();
    }
    else if(strcmp(params.bp_name, "gshare") == 0){
        std::cout << "number of predictions: " << gshared_total_counter << std::endl;
        std::cout << "number of mispredictions: " << gshared_miss_counter << std::endl;
        double result = (static_cast<double>(gshared_miss_counter)/gshared_total_counter)*100;
        printf("misprediction rate: %.2lf%%\n", result);

        std::cout << "FINAL GSHARE CONTENTS" << std::endl;
        bp_gshared.display();
    }
    else if(strcmp(params.bp_name, "hybrid") == 0){
        std::cout << "number of predictions: " << hybrid_total_counter << std::endl;
        std::cout << "number of mispredictions: " << hybrid_miss_counter << std::endl;
        double result = (static_cast<double>(hybrid_miss_counter)/hybrid_total_counter)*100;
        printf("misprediction rate: %.2lf%%\n", result);

        bp_hybrid.display_selector();

        std::cout << "FINAL GSHARE CONTENTS" << std::endl;
        bp_gshared.display();

        std::cout << "FINAL BIMODAL CONTENTS" << std::endl;
        bp_bimodal.display();
    }

    return 0;
}
