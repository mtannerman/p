#include "../inc/tests.h"
#include "../inc/SimpleParserTest.h"


void RunTests()
{
	

    ADD_TEST("quote in quote 1", {
        return SimpleParserTest("\"aaa\",\"b\"\"bb\",\"222, 333\"\n",
            {{{"aaa"},{"b\"bb"},{"222, 333"}}}).Evaluate();
    }); 

	ADD_TEST("newline in quotes", {
		return SimpleParserTest("\"aa\naa\"\n",
			{{{ "aa\naa" }}}).Evaluate();
	});

	ADD_TEST("newline in quotes without ending newline", {
		return SimpleParserTest("\"aa\naa\"",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("newline in quotes with CRLF", {
		return SimpleParserTest("\"aa\naa\"\r\n",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("CRLF in quotes", {
		return SimpleParserTest("\"aa\r\naa\"",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("CRLF in quotes with CR ending", {
		return SimpleParserTest("\"aa\r\naa\"\r",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("empty input", {
		return SimpleParserTest("",
		std::vector<std::vector<std::string>>()).Evaluate();
	});

	ADD_TEST("empty field", {
		return SimpleParserTest("a,b,,d",
		{{"a","b","","d"}}).Evaluate();
	});

	ADD_TEST("mixed 1", {
		return SimpleParserTest("a,b,,d\naa\r\nb,\"a\nb\"",
		{{"a","b","","d"}, {"aa"}, {"b", "a\nb"}}).Evaluate();
	});

	ADD_TEST("double quote in quoted field", {
		return SimpleParserTest("\" Simple Hill \"\"\"\"6\"",
		{{" Simple Hill \"\"6"}}).Evaluate();
	});

    testutil::TestManager::GetInstance().RunAll();   
}