#include <bits/stdc++.h>

#define REP(i, N) for (int i=0; i<(int)N; ++i)
#define ALL(x) (x).begin(),(x).end()
using namespace std;

typedef long long ll;

${<if Problem.Description.Modulo}
static const int mod = ${Problem.Description.Modulo};
${<end}

class ${ClassName} {
 public:
  ${Method.ReturnType} ${Method.Name}(${Method.Params}) {
    ${Method.ReturnType} res = ${Method.ReturnType;zeroval};
    
    return res;
  }
};

${CutBegin}
${<TestCode}
${CutEnd}
