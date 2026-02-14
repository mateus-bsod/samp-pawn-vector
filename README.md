# samp-pawn-vector
Vector for the pawn

```pawn
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

```

#### Example code
```pawn

#include <a_samp>
#include <vector>

main(){}


public OnGameModeInit()
{
	new Vector:v = vector_create();

	// Adding values
	vector_add_int(v, 10); // 0
	vector_add_string(v, "Mateus"); // 1
	vector_add_string(v, "BSOD"); // 2
	vector_add_string(v, "mateus-bsod"); // 3
	vector_add_string(v, "System of a Down"); // 4
	vector_add_string(v, "System"); // 5
	vector_add_string(v, "Xereca Mateus"); // 6
	vector_add_float(v, 6.66); // 7

	printf("\n\n");
	DebugVector(v);

	vector_set_index(v, 4, "System of Feudal");

	printf("\n\n");
	DebugVector(v);

	vector_insert_str(v, 4, "System antes do Feudal");

	printf("\n\n");
	DebugVector(v);

	printf("\nVector Size: %d", vector_size(v));

	vector_free(v);
    return 1;
}

stock DebugVector(Vector:v)
{
	new type, dest[120] = "Nada", value, Float:value2;
	for(new i = 0; i < vector_size(v); i++)
	{
		if(vector_contains_index(v, i))
		{
			type = vector_get_index_type(v, i);
			switch(type)
			{
				case TYPE_INT:
				{
					vector_get_int(v, i, value);
					printf("Index %02d: %d", i, value);
				}
				case TYPE_FLOAT:
				{
					vector_get_float(v, i, value2);
					printf("Index %02d: %f", i, value2);
				}
				case TYPE_STRING:
				{
					vector_get_string(v, i, dest, sizeof(dest));
					printf("Index %02d: %s", i, dest);
				}
			}
		}
	}
}
```

#### Output
```
Index 00: 10
Index 01: Mateus
Index 02: BSOD
Index 03: mateus-bsod
Index 04: System of a Down
Index 05: System
Index 06: Xereca Mateus
Index 07: 6.659999



Index 00: 10
Index 01: Mateus
Index 02: BSOD
Index 03: mateus-bsod
Index 04: System of Feudal
Index 05: System
Index 06: Xereca Mateus
Index 07: 6.659999



Index 00: 10
Index 01: Mateus
Index 02: BSOD
Index 03: mateus-bsod
Index 04: System antes do Feudal
Index 05: System of Feudal
Index 06: System
Index 07: Xereca Mateus
Index 08: 6.659999

Vector Size: 9
```
