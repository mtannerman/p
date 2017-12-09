#include "../inc/tests.h"
#include "../inc/SimpleParserTest.h"
#include "../inc/ExceptionAnticipatingTest.h"

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

	ADD_TEST("combination of corner cases 1", {
		return SimpleParserTest("a,b,,d\naa\r\nb,\"a\nb\"",
		{{"a","b","","d"}, {"aa"}, {"b", "a\nb"}}).Evaluate();
	});

	ADD_TEST("combination of line endings", {
		return SimpleParserTest("a,a,a,a\r\nb,b,b,b\nc,c,c,c\rd,d,d,d\r\n",
		{{"a","a","a","a"},{"b","b","b","b"},{"c","c","c","c"}, {"d","d","d","d"}}).Evaluate();
	});

	ADD_TEST("trailing blank line", {
		return SimpleParserTest("aaa\n\naaa",
		{{"aaa"},{""},{"aaa"}}).Evaluate();
	});

	ADD_TEST("double quote in quoted field", {
		return SimpleParserTest("\" Simple Hill \"\"\"\"6\"",
		{{" Simple Hill \"\"6"}}).Evaluate();
	});

	ADD_TEST("different number of fields", {
		return SimpleParserTest("aaa,aa,a\naa,a\na",
		{{"aaa","aa","a"},{"aa", "a"},{"a"}}).Evaluate();
	});

	ADD_TEST("quoted field with comma", {
		return SimpleParserTest("\" Simple,,, Hill \"\"\"\"6\"",
		{{" Simple,,, Hill \"\"6"}}).Evaluate();
	});

	ADD_TEST("quote exception 1", {
		return ExceptionAnticipatingTest("\"\"\"\n", "Quote mismatch.").Evaluate();
	});

	ADD_TEST("quote exception 2", {
		return ExceptionAnticipatingTest("\"\"\"\"\"\n", "Quote mismatch.").Evaluate();
	});

    testutil::TestManager::GetInstance().RunAll();   
}

