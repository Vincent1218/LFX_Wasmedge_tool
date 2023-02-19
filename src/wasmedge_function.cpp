#include <wasmedge/wasmedge.h>

using namespace std;

void printVersion() {
    cout << "WasmEdge version: " << WasmEdge_VersionGet() << endl;
}

int loadWasm(string wasm_path, WasmEdge_VMContext *VMCxt) {
  // Load .wasm file
  cout << "Loading WASM file: " << wasm_path << endl;
  WasmEdge_Result res = WasmEdge_VMLoadWasmFromFile(VMCxt, wasm_path.c_str());
  if (WasmEdge_ResultOK(res) != true) {
      cerr << "Error: Unable to load Wasm file: " << WasmEdge_ResultGetMessage(res) << endl;
      return 1;
  }
  // Validate the WASM module.
  res = WasmEdge_VMValidate(VMCxt);
  if (!WasmEdge_ResultOK(res)) {
      cerr << "Error: Validation phase failed: " << WasmEdge_ResultGetMessage(res) << endl;
      return 1;
  }
  // Instantiate the WASM module.
  res = WasmEdge_VMInstantiate(VMCxt);
  if (!WasmEdge_ResultOK(res)) {
      cerr << "Error: Instantiation phase failed: " << WasmEdge_ResultGetMessage(res) << endl;
      return 1;
  }

  cout << "WASM file loaded successfully" << endl << endl;

  return 0;
}

int processWasm(WasmEdge_VMContext *VMCxt, vector<string> args){  
  cout << "Processing WASM file" << endl;

  // Get the number of exported functions.
  int funcNo = WasmEdge_VMGetFunctionListLength(VMCxt);
  cout << "Number of exported functions: " << funcNo << endl << endl;

  // Get function type and name.
  WasmEdge_String funcNameList[funcNo];
  const WasmEdge_FunctionTypeContext *funcTypesList[funcNo];
  WasmEdge_VMGetFunctionList(VMCxt, funcNameList, funcTypesList, funcNo);

  // When running multiple functions, this will be used to keep track of the arguments
  int counterForArgs = 0;

  int totalParam = 0;

  // Check sum of param length of function list and argument list length
  cout << "Checking sum of param length of function list and argument list length" << endl;
  for (int i = 0; i < funcNo; i++) {
    int paramLen = WasmEdge_FunctionTypeGetParametersLength(funcTypesList[i]);
    totalParam += paramLen;
  }
  cout << "Total number of parameters: " << totalParam << endl;
  cout << "Toral number of application argument: " << args.size() << endl;
  if (totalParam != args.size()) {
    cerr << "Error: Number of arguments does not match sum of parameters of all functions." << endl;
    return 1;
  }
  cout << endl;
  
  // Loop through the functions.
  for (int i = 0; i < funcNo; i++) {
    // Get the number of parameters and returns."
    int paramLen = WasmEdge_FunctionTypeGetParametersLength(funcTypesList[i]);
    int retLen = WasmEdge_FunctionTypeGetReturnsLength(funcTypesList[i]);

    // Allocate memory for the parameter and return types.
    enum WasmEdge_ValType paramBuf[paramLen];
    enum WasmEdge_ValType returnBuf[retLen];

    // Get the parameter and return types.
    WasmEdge_FunctionTypeGetParameters(funcTypesList[i], paramBuf, paramLen);
    WasmEdge_FunctionTypeGetReturns(funcTypesList[i], returnBuf, retLen);

    // Allocate memory for the parameters and returns.
    WasmEdge_Value params[paramLen];
    WasmEdge_Value returns[retLen];

    // Set the parameters.
    // Only support I32, F32 for now.
    if (paramLen > 0) {
      cout << "Input values: " << endl;
    }
    for (int i = 0; i < paramLen; i++) {
      switch(paramBuf[i]){
        case WasmEdge_ValType_I32:
          // If string is float, it will be rounded to int
          params[i] = WasmEdge_ValueGenI32(stoi(args[counterForArgs]));
          cout << WasmEdge_ValueGetI32(params[i]) << endl;
          break;
        case WasmEdge_ValType_F32:
          params[i] = WasmEdge_ValueGenF32(stof(args[counterForArgs]));
          cout << WasmEdge_ValueGetF32(params[i]) << endl;
          break;
        default:
          params[i] = WasmEdge_ValueGenI32(0);
          cerr << "Error: Unsupported type, params value set to 0" << endl;
          break;
      }
      counterForArgs++;
    }
    cout << endl;

    // Execute the function.
    cout << "Executing function: " << funcNameList[i].Buf  << endl;
    WasmEdge_Result res = WasmEdge_VMExecute(VMCxt, funcNameList[i], params, paramLen, returns, retLen);

    if (WasmEdge_ResultOK(res)) {
      cout << "Execution phase succeeded" << endl;
    } else {
      cout << "Error: Execution phase failed: " << WasmEdge_ResultGetMessage(res) << endl;
    }
    cout << endl;

    // Print the return values.
    if (retLen > 0) {
      cout << "Return values: " << endl;
    }
    for (int i = 0; i < retLen; i++) {
      switch(returnBuf[i]){
        case WasmEdge_ValType_I32:
          cout << WasmEdge_ValueGetI32(returns[i]) << endl;
          break;
        case WasmEdge_ValType_F32:
          cout << WasmEdge_ValueGetF32(returns[i]) << endl;
          break;
        default:
          cerr << "Error: Unsupported type" << endl;
          break;
      }
    }
  }
  cout << endl;

  // Delete the function types and names.
  for (int i = 0; i < funcNo; i++) {
    WasmEdge_StringDelete(funcNameList[i]);
  }

  return 0;
}

int vmMain(string wasm_path, vector<string> args) {
  // Initialize WasmEdge VM 
  // Create the configure context and add the WASI support.
  WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
  WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
  // The configure and store context to the VM creation can be NULL.
  WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);

  // Load .wasm file
  if (loadWasm(wasm_path, VMCxt) != 0) {
    return 1;
  }
  
  // Process .wasm file
  if (processWasm(VMCxt, args) != 0) {
    return 1;
  }

  // Resource deallocation
  WasmEdge_VMDelete(VMCxt);
  WasmEdge_ConfigureDelete(ConfCxt);

  return 0;
}