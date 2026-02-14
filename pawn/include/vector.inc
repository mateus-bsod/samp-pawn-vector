/*
                                                              
         /$$    /$$                      /$$                        
        | $$   | $$                     | $$                        
        | $$   | $$ /$$$$$$   /$$$$$$$ /$$$$$$    /$$$$$$   /$$$$$$ 
        |  $$ / $$//$$__  $$ /$$_____/|_  $$_/   /$$__  $$ /$$__  $$
         \  $$ $$/| $$$$$$$$| $$        | $$    | $$  \ $$| $$  \__/
          \  $$$/ | $$_____/| $$        | $$ /$$| $$  | $$| $$      
           \  $/  |  $$$$$$$|  $$$$$$$  |  $$$$/|  $$$$$$/| $$      
            \_/    \_______/ \_______/   \___/   \______/ |__/      
                                                                    
    -» Author    	       : Mateus Mesquita Lima / mateus-bsod
    -» Release Date	       : 14 Fev 2026
    -» Description	       : Include / Plugin

*/

#if defined _Vector_included
    #endinput
#endif
#define _Vector_included

/*

                    _____  __ __  ____     __ ______  ____  ___   ____   _____
                    |     ||  |  ||    \   /  ]      ||    |/   \ |    \ / ___/
                    |   __||  |  ||  _  | /  /|      | |  ||     ||  _  (   \_ 
                    |  |_  |  |  ||  |  |/  / |_|  |_| |  ||  O  ||  |  |\__  |
                    |   _] |  :  ||  |  /   \_  |  |   |  ||     ||  |  |/  \ |
                    |  |   |     ||  |  \     | |  |   |  ||     ||  |  |\    |
                    |__|    \__,_||__|__|\____| |__|  |____|\___/ |__|__| \___|

                    ___________________________________________________________                           

*/


enum //VECTOR_TYPES
{
    TYPE_INT = 0,
    TYPE_FLOAT = 1, 
    TYPE_STRING = 2
}

native Vector:vector_create();
native vector_add_int(Vector:handle, value);
native vector_add_float(Vector:handle, Float:value);
native vector_add_string(Vector:handle, const text[]);
native vector_size(Vector:handle);
native vector_free(Vector:handle);

// get vector
native vector_get_int(Vector:handle, index, &value);
native vector_get_float(Vector:handle, index, &Float:value);
native vector_get_string(Vector:handle, index, dest[], size);
native vector_get_index_type(Vector:handle, index);

// Manipulating index vectors
native bool:vector_contains_index(Vector:handle, index);
native vector_get_index_st(Vector:handle, const text[]);
native vector_insert_str(Vector:handle, index, const text[]);
native vector_set_index(Vector:handle, index, const text[]);
