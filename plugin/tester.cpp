//------------------------------------------------------------------------------

string do_test(${foreach Method.Params p}${p.Type},${end}${Method.ReturnType},int,bool);

const double CASE_TIME_OUT = 2.0;
 
string run_testcase(int __no) {
${<foreach Method.Params p}
  ${if p.Type.Array}vector<${p.Type.Primitive}>${else}${p.Type.Primitive}${end} ${p.Name};
${<end}
  ${if Method.ReturnType.Array}vector<${Method.ReturnType.Primitive}>${else}${Method.ReturnType.Primitive}${end} __expected = ${Method.ReturnType;zeroval};
  bool __unknownAnswer = false;

  switch (__no) {
${<foreach Examples e}
    case ${e.Num}: {
${<foreach e.Input in}
${<if !in.Param.Type.Array}
      ${in.Param.Name} = ${in};
${<else}
      ${in.Param.Name} = { ${foreach in.ValueList v ,}${v}${end} };
${<end}
${<end}
${<if !e.Output.Param.Type.Array}
      __expected = ${e.Output};
${<else}
      __expected = {${foreach e.Output.ValueList v ,} ${v}${end} };
${<end}
      break;
    }
${<end}
    default: return "Error: There is not such a test case.";
  }
  return do_test(${foreach Method.Params p , }${p.Name}${end}, __expected, __no, __unknownAnswer);
}

int      numTests = ${NumOfExamples};
set<int> runTests = { ${foreach Examples e , }${e.Num}${end} };

template <typename T> string pretty_print(T t) {
  stringstream s;
  typeid(T) == typeid(string) ? s << "\\"" << t << "\\"" : s << t;
  return s.str();
}

${<if HasArray}
// Vector print
template <typename T> ostream &operator << (ostream &out, vector<T> arr) {
  out << "{";
  for (int i = 0; i < (int)arr.size(); ++i)
    out << (i == 0 ? "" : ",") << pretty_print(arr[i]);
  out << "}";
  return out;
}
${<end}

${<if Method.ReturnType.RealNumber}
// Double comparison
bool double_equal (const double &e, const double &r) {
  return abs(e - r) < 1e-9 || abs(r) > abs(e) * (1.0 - 1e-9) && abs(r) < abs(e) * (1.0 + 1e-9);
}

${<if Method.ReturnType.Array}
// Vector Double comparison
bool double_vector_equal (const vector<double> &e, const vector<double> &r) {
  if (e.size() != r.size()) return false;
  int n = e.size();
  for (int i = 0; i < n; ++i)
    if (!double_equal(e[i], r[i])) return false;
  return true;
}
${<end}
${<end}

// Coloring
string colorString(string s, string color) {
  if (color == "K") return "\\033[1;30m"+s+"\\033[0m";
  if (color == "R") return "\\033[1;31m"+s+"\\033[0m";
  if (color == "G") return "\\033[1;32m"+s+"\\033[0m";
  if (color == "Y") return "\\033[1;33m"+s+"\\033[0m";
  if (color == "B") return "\\033[1;34m"+s+"\\033[0m";
  if (color == "M") return "\\033[1;35m"+s+"\\033[0m";
  if (color == "C") return "\\033[1;36m"+s+"\\033[0m";
  return s;
}

string colorTestResult(string s) {
  if (s[0] == 'P') return colorString(s, "G");
  if (s[0] == 'W') return colorString(s, "R");
  if (s[0] == 'T') return colorString(s, "Y");
  if (s[0] == 'P') return colorString(s, "W");
  return "";
}

string do_test(${Method.Params}, ${Method.ReturnType} __expected, int __caseNo, bool __unknownAnswer) {
  cout << colorString("Test " + to_string(__caseNo), "C");
  cout << ": [" ${<foreach Method.Params p << ","}<< ${if p.Type.Array}${p.Name}${else}pretty_print(${p.Name})${end} ${<end} << "]" << endl;
  time_t __startClock = clock();
  ${ClassName} *__instance = new ${ClassName}();
  ${Method.ReturnType} __result = __instance->${Method.Name}(${foreach Method.Params par , }${par.Name}${end});
  double __elapsed = (double)(clock() - __startClock) / CLOCKS_PER_SEC;
  cout << "Time: "<< (int)(1000.0 * __elapsed) <<" ms." << endl;
  delete __instance;
  bool __correct = __unknownAnswer;
${<if Method.ReturnType.RealNumber}
${<if Method.ReturnType.Array}
  __correct |= double_vector_equal(__expected, __result);
${<else}
  __correct |= double_equal(__expected, __result);
${<end}
${<else}
  __correct |= (__result == __expected);
${<end}
  if (! __correct) {
    cout << "Expected:" << endl;
    cout << "\\t" << ${if Method.ReturnType.Array} __expected ${else} pretty_print(__expected) ${end} << endl;
  }
  cout << "Returns:" << endl;
  cout << "\\t" << ${if Method.ReturnType.Array} __result ${else} pretty_print(__result) ${end} << endl; 
  string __res = "Pending";
  if (! __correct) __res = "Wrong Answer";
  else if (__elapsed > CASE_TIME_OUT ) __res = "Time Limit Exceeded";
  else if (__unknownAnswer) __res = "Judge Not Available";
  else __res = "Passed";
  cout << " " << colorTestResult(__res) << endl;
  cout << colorString("===============================================================", "K") << endl;
  return __res;
}

int main(int argc, char *argv[]) {
  string result = "Passed Pretest";
  bool good = true;
  for (int i : runTests) {
    string s = run_testcase(i);
    if (s[0] == 'W' || s[0] == 'T') { result = "Failed Pretest"; good = false; }
  }
  cout << "${Problem.Name}: " << colorString(result, good ? "G" : "R");
  int T = time(NULL) - ${CreateTime};
  double PT = T / 60.0, TT = 75.0, SS = ${Problem.Score} * (0.3 + (0.7 * TT * TT) / (10.0 * PT * PT + TT * TT));
  string SSS = to_string((int)SS) + "." + to_string((int)(SS*100) % 100);
  cout << " (" << colorString(SSS, good ? "G" : "R") << ")." << endl;
  return 0;
}
