check("1", tostring(test0(0)));
check("2", tostring(test1(0)));
check("3.0", tostring(test2(0)));
check("4.0", tostring(test3(0)));
check("false", tostring(test4(true)));
check("abcLUA", tostring(test5("abc")));
check("Lef", tostring(test6("def")));
check("B", tostring(test7(65)));
