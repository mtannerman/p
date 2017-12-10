#include "../inc/tests.h"
#include "../inc/SimpleParserTest.h"
#include "../inc/ExceptionAnticipatingTest.h"

void RunTests()
{
	ADD_TEST("newline in quotes with CRLF", {
		return SimpleCSVParserTest("\"aa\naa\"\r\n",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("CRLF in quotes with CRLF", {
		return SimpleCSVParserTest("\"aa\r\naa\"\r\n",
		{{{"aa\naa"}}}).Evaluate();
	});

    ADD_TEST("quote in quote 1", {
        return SimpleCSVParserTest("\"aaa\",\"b\"\"bb\",\"222, 333\"\n",
            {{{"aaa"},{"b\"bb"},{"222, 333"}}}).Evaluate();
    }); 

	ADD_TEST("newline in quotes", {
		return SimpleCSVParserTest("\"aa\naa\"\n",
			{{{ "aa\naa" }}}).Evaluate();
	});

	ADD_TEST("semicolon delimiter test", {
		return SimpleCSVParserTest("a;b;c;d",
		{{{"a", "b", "c", "d"}}}, ';').Evaluate();
	});

	ADD_TEST("semicolon quotation test", {
		return SimpleCSVParserTest(";a;,;b;,cd",
		{{{"a", "b", "cd"}}}, ',', ';').Evaluate();
	});

	ADD_TEST("newline in quotes without ending newline", {
		return SimpleCSVParserTest("\"aa\naa\"",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("CRLF in quotes", {
		return SimpleCSVParserTest("\"aa\r\naa\"",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("CRLF in quotes with CR ending", {
		return SimpleCSVParserTest("\"aa\r\naa\"\r",
		{{{"aa\naa"}}}).Evaluate();
	});

	ADD_TEST("empty input", {
		return SimpleCSVParserTest("",
		std::vector<std::vector<std::string>>()).Evaluate();
	});

	ADD_TEST("empty field", {
		return SimpleCSVParserTest("a,b,,d",
		{{"a","b","","d"}}).Evaluate();
	});

	ADD_TEST("combination of corner cases 1", {
		return SimpleCSVParserTest("a,b,,d\naa\r\nb,\"a\nb\"",
		{{"a","b","","d"}, {"aa"}, {"b", "a\nb"}}).Evaluate();
	});

	ADD_TEST("combination of line endings", {
		return SimpleCSVParserTest("a,a,a,a\r\nb,b,b,b\nc,c,c,c\rd,d,d,d\r\n",
		{{"a","a","a","a"},{"b","b","b","b"},{"c","c","c","c"}, {"d","d","d","d"}}).Evaluate();
	});

	ADD_TEST("trailing blank line", {
		return SimpleCSVParserTest("aaa\n\naaa",
		{{"aaa"},{""},{"aaa"}}).Evaluate();
	});

	ADD_TEST("double quote in quoted field", {
		return SimpleCSVParserTest("\" Simple Hill \"\"\"\"6\"",
		{{" Simple Hill \"\"6"}}).Evaluate();
	});

	ADD_TEST("different number of fields", {
		return SimpleCSVParserTest("aaa,aa,a\naa,a\na",
		{{"aaa","aa","a"},{"aa", "a"},{"a"}}).Evaluate();
	});

	ADD_TEST("quoted field with comma 1", {
		return SimpleCSVParserTest("\",\",asd",
		{{",", "asd"}}).Evaluate();
	});

	ADD_TEST("quoted field with comma 2", {
		return SimpleCSVParserTest("\" Simple,,, Hill \"\"\"\"6\"",
		{{" Simple,,, Hill \"\"6"}}).Evaluate();
	});

	ADD_TEST("quote exception 1", {
		return ExceptionAnticipatingTest("\"\"\"\n", "Quote mismatch.").Evaluate();
	});

	ADD_TEST("quote exception 2", {
		return ExceptionAnticipatingTest("\"\"\"\"\"\n", "Quote mismatch.").Evaluate();
	});

	ADD_TEST("quote exception 3", {
		return ExceptionAnticipatingTest("\",", "Quote mismatch.").Evaluate();
	});

	ADD_TEST("bad delimiter exception 1", {
		return ExceptionAnticipatingTest("dummy", "Bad delimiter argument.", '\r').Evaluate();
	});

	ADD_TEST("bad delimiter exception 2", {
		return ExceptionAnticipatingTest("dummy", "Bad delimiter argument.", '\n').Evaluate();
	});

	ADD_TEST("bad quotation exception 1", {
		return ExceptionAnticipatingTest("dummy", "Bad quotation argument.", ';', '\r').Evaluate();
	});

	ADD_TEST("bad quotation exception 2", {
		return ExceptionAnticipatingTest("dummy", "Bad quotation argument.", ';', '\n').Evaluate();
	});

	ADD_TEST("delimiter = quotation exception", {
		return ExceptionAnticipatingTest("dummy", "Bad argument. Quotation equals delimiter.", ';', ';').Evaluate();
	});

    testutil::TestManager::GetInstance().RunAll();   
}

