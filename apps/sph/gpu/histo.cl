kernel void collect(read_only global int* input, write_only global int* output,
                       const int inputLength, const int factor){
   int i = get_global_id(0);
   if(i == 0){
       output[i] = 0;
       return;
   }
   int res = 0;
   int start = (i - 1) * factor;
   for(int j = 0; j < factor; j++){
       if(start + j < inputLength){
           res += input[start + j];
       }
   }
   output[i] = res;
}


kernel void expand(read_only global int* input, read_only global int* collectedInput, write_only global int* output,
                       const int inputLength, const int factor){
   int i = get_global_id(0);
   int accum = collectedInput[i];
   int start = i * factor;
   for(int j = 0; j < factor; j++){
       if(start + j < inputLength){
           output[start + j] = accum;
           accum += input[start + j];
       }
   }
}

kernel void expandInternal(read_only global int* input, read_only global int* collectedInput, write_only global int* output,
                       const int inputLength, const int factor){
   int i = get_global_id(0);
   int accum = collectedInput[i];
   int start = i * factor;
   for(int j = 0; j < factor; j++){
       if(start + j < inputLength){
           accum += input[start + j];
           output[start + j] = accum;
       }
   }
}

kernel void histo(read_only global int* input, write_only global int* output, const int length){
   int accum = 0;
   for(int i = 0; i < length; i++){
       output[i] = accum;
       accum += input[i];
   }
}

kernel void histoInternal(read_only global int* input, write_only global int* output, const int length){
   int accum = 0;
   for(int i = 0; i < length; i++){
       accum += input[i];
       output[i] = accum;
   }
}
