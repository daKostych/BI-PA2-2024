sed -n -e '/^#endif/q' -e '/^#include "/!p' test.cpp > all.cpp
echo "#endif /* __PROGTEST__ */" >> all.cpp
sed '2i #include "expression.h"' all.cpp > all_in_one.cpp
rm all.cpp
echo "using namespace std;" >> all_in_one.cpp
echo "class Node;" >> all_in_one.cpp
echo "using ANode = unique_ptr<Node>;" >> all_in_one.cpp

grep -vh -e '^#' -e '^using namespace std;' position.h position.cpp >> all_in_one.cpp

grep -vh -e '^#' -e '^using CValue' -e '^class Node;' -e '^using ANode' node.h node.cpp >> all_in_one.cpp

grep -vh '^#' operators.h operators.cpp >> all_in_one.cpp

grep -vh '^#' functions.h functions.cpp >> all_in_one.cpp

grep -vh -e '^#' -e '^using' builder.h builder.cpp >> all_in_one.cpp

grep -vh -e '^#' -e '^using' -e '^constexpr' spreadSheet.h spreadSheet.cpp >> all_in_one.cpp

awk '/^#ifndef/{c++} c==2 {print}' test.cpp >> all_in_one.cpp