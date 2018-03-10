#include "cpu.h"

CPU::CPU()
{
  tableSize=5329;
  tempAddr=0; mybuffer1=0; mybuffer2=0; 
  index1=0; index2=0; index3=0;
  last=0;
  a=0;b=0;
  //arr = new cacheBlock [tableSize];
} // CPU()


void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer)
{
  switch (opCode)
  {
    case ADD  :
      a=0;b=0;
      index1 = instruction.addr1 % tableSize;
      if(arr[index1].tag != instruction.addr1)
      {
	      if(arr[index1].isValid ==0)
	      {
		      opCode = LOAD; break;
	      }
	      else
	      {
		      tempAddr = instruction.addr1;
      	  	      instruction.addr1 = arr[index1].tag;
      	  	      buffer = arr[index1].value;
      	  	      opCode = STORE; 
      	  	      break;
      	      }
      }
      else //address incoming is same as address stored
      {
	      mybuffer1 = arr[index1].value;
	      a=0;b=1;
      }
		      
        
      index2 = instruction.addr2 % tableSize;
      instruction.addr1 = instruction.addr2;
      
      if(arr[index2].tag != instruction.addr2)
      {
              if(arr[index2].isValid ==0)
              {
        	      opCode = LOAD; break;
              }
              else
              {
        	      tempAddr = instruction.addr1;
      	  	      instruction.addr1 = arr[index2].tag;
      	  	      buffer = arr[index2].value;
      	  	      opCode = STORE; 
      	  	      break;
      	      }
      }
      else //address incoming is same as address stored
      {
              mybuffer2 = arr[index2].value;
	      a=1;b=0;
      }

      index3 = instruction.addr3 % tableSize;
      instruction.addr1 = instruction.addr3;

      if(arr[index3].tag != instruction.addr3)
      {
              if(arr[index3].isValid ==0)
              {
        		arr[index3].isValid = 1;
        		arr[index3].tag = instruction.addr3;
        		arr[index3].value = mybuffer1+mybuffer2;
        		opCode = ADD; break; 
              }
              else
              {
        	      tempAddr = instruction.addr1;
      	  	      instruction.addr1 = arr[index3].tag;
      	  	      buffer = arr[index3].value;
      	  	      opCode = STORE; 
      	  	      break;
      	      }
      }
      else //address incoming is same as address stored
      {
	      arr[index3].isValid = 1;
      	      arr[index3].value = mybuffer1 + mybuffer2;
      }
      opCode = ADD;
      break; // *addr3 = *addr1 + *addr2
    
    case LOAD :  
      if(!a && !b)
      {
        arr[index1].isValid = 0;
        arr[index1].tag = instruction.addr1;
        arr[index1].value = buffer;
        mybuffer1 = buffer;

	a=0; b=1;
        index2 = instruction.addr2 % tableSize;
        instruction.addr1 = instruction.addr2;
      
      if(arr[index2].tag != instruction.addr2)
      {
              if(arr[index2].isValid ==0)
              {
        	      opCode = LOAD; break;
              }
              else
              {
        	      tempAddr = instruction.addr1;
      	  	      instruction.addr1 = arr[index2].tag;
      	  	      buffer = arr[index2].value;
      	  	      opCode = STORE; 
      	  	      break;
      	      }
      }
      else
      {
              mybuffer2 = arr[index2].value;
	      a=1;b=0;
      }

      index3 = instruction.addr3 % tableSize;
      instruction.addr1 = instruction.addr3;

      if(arr[index3].tag != instruction.addr3)
      {
              if(arr[index3].isValid ==0)
              {
        		arr[index3].isValid = 1;
        		arr[index3].tag = instruction.addr3;
        		arr[index3].value = mybuffer1+mybuffer2;
        		opCode = ADD; break; 
              }
              else
              {
        	      tempAddr = instruction.addr1;
      	  	      instruction.addr1 = arr[index3].tag;
      	  	      buffer = arr[index3].value;
      	  	      opCode = STORE; 
      	  	      break;
      	      }
      }
      else 
      {
	      arr[index3].isValid = 1;
      	      arr[index3].value = mybuffer1 + mybuffer2;
      }
      opCode = ADD;
	  break;
      }

      if(!a && b)
      {
        arr[index2].isValid = 0;
        arr[index2].tag = instruction.addr2;
        arr[index2].value = buffer;
        mybuffer2 = buffer;
       
	a=1;b=0;
        index3 = instruction.addr3 % tableSize;
      instruction.addr1 = instruction.addr3;

      if(arr[index3].tag != instruction.addr3)
      {
              if(arr[index3].isValid ==0)
              {
        		arr[index3].isValid = 1;
        		arr[index3].tag = instruction.addr3;
        		arr[index3].value = mybuffer1+mybuffer2;
        		opCode = ADD; break; 
              }
              else
              {
        	      tempAddr = instruction.addr1;
      	  	      instruction.addr1 = arr[index3].tag;
      	  	      buffer = arr[index3].value;
      	  	      opCode = STORE; 
      	  	      break;
      	      }
      }
      else 
      {
	      arr[index3].isValid = 1;
      	      arr[index3].value = mybuffer1 + mybuffer2;
      }
      opCode = ADD;
        break;
      }
 
      if(a && !b)
      {
        arr[index3].isValid = 1;
        arr[index3].tag = instruction.addr3;
        arr[index3].value = mybuffer1+mybuffer2;
        opCode = ADD; break; 
       }// buffer contains int requested from RAM
    
    case STORE:  
      if(!a && !b)
      { 
        instruction.addr1 = tempAddr; 
        opCode = LOAD; 
        break; 
      }

      if(!a && b)
      { opCode = LOAD; instruction.addr1 = tempAddr; break; }
 
      if(a && !b)
      { 
        arr[index3].isValid = 1;
        arr[index3].tag = tempAddr;//instruction.addr3;
        arr[index3].value = mybuffer1+mybuffer2;
        opCode = ADD; break; 
      }      

      if(a && b)
      {
	for(;last<tableSize;last++)
        {
          if(arr[last].isValid)
          {
            instruction.addr1 = arr[last].tag;
            buffer = arr[last].value;
            arr[last].isValid=0;
            break;
          }
        }
        if(last>=tableSize)
          { opCode=DONE; 
	    break;
	  }

      break; // Sent by RAM after a STORE
      }
    case DONE :
      a=1;b=1;
      for(last=0;last<tableSize;last++)
      {
        if(arr[last].isValid != 0)
        {
          instruction.addr1 = arr[last].tag;
          buffer = arr[last].value;
          opCode = STORE;
	  arr[last].isValid = 0;
          break;
        }
      }
      break;  //storing valid cache
  } // switch
} // operation()
