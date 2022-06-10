#include <vector>
#include <string>
#include <iostream>

class Value{
    public:
        uint32_t float_value;
        std::string string_value;

        uint32_t vector2_value_x;
        uint32_t vector2_value_y;

        uint32_t vector3_value_x;
        uint32_t vector3_value_y;
        uint32_t vector3_value_z;

        int int_value;
};

class Variant{
    private:
        int count;
        int offset;
        int vector_size;
        std::vector<uint8_t> variant_list;

        void reserve(int size){
            this->vector_size += size;
            this->variant_list.reserve(this->vector_size);
        }

    public:
        Variant(){
            this->count = 0;
            this->offset = 1;
            this->vector_size = 1;

            this->variant_list.reserve(this->vector_size);
            memcpy(this->variant_list.data(), &this->count, 1);
        }

        enum {
                NONE = 0,
                FLOAT = 1,
                STRING = 2,
                VECTOR2 = 3,
                VECTOR3 = 4,
                INT32 = 9
            };

        uint8_t* get_variant_list(){
            //please lmk if variant_list.data() is automatically on POS 0, cuz here IDFK WHAT I'M DOING LIKE ADDING 0 BREV WTF
            memcpy(this->variant_list.data() + 0, &this->count, 1);
            return variant_list.data(); 
        }

        void append(int type, Value* value){
            this->reserve(2);

            memcpy(this->variant_list.data() + this->offset, &this->count, 1); //index
            this->offset += 1;

            memcpy(this->variant_list.data() + this->offset, &type, 1); //variant type
            this->offset += 1;
            
            switch(type){
                case FLOAT:
                    this->reserve(4);
                    memcpy(this->variant_list.data() + this->offset, &value->float_value, 4); this->offset += 4;
                    break;
                case VECTOR2:
                    this->reserve(8);
                    memcpy(this->variant_list.data() + this->offset, &value->vector2_value_x, 4); this->offset += 4;
                    memcpy(this->variant_list.data() + this->offset, &value->vector2_value_y, 4); this->offset += 4;
                    break;
                case VECTOR3:
                    this->reserve(12);
                    memcpy(this->variant_list.data() + this->offset, &value->vector3_value_x, 4); this->offset += 4;
                    memcpy(this->variant_list.data() + this->offset, &value->vector3_value_y, 4); this->offset += 4;
                    memcpy(this->variant_list.data() + this->offset, &value->vector3_value_z, 4); this->offset += 4;
                    break;
                case INT32:
                    this->reserve(4);

                    memcpy(this->variant_list.data() + this->offset, &value->int_value, 4); this->offset += 4;
                    break;
                case STRING:
                    this->reserve(value->string_value.size() + 4);
                    int string_size = value->string_value.length();
                    
                    memcpy(this->variant_list.data() + this->offset, &string_size, 4); this->offset += 4;
                    memcpy(this->variant_list.data() + this->offset, value->string_value.data(), string_size); this->offset += string_size;
                    break;
            }

            this->count += 1;
        }
};