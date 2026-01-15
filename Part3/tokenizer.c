// Simple tokenizer for a small C-like grammar.
// Restrictions: no <string.h> helpers except strlen (not used here).

#include <stdio.h>

#define MAX_TOKEN_LEN 1024

static int is_letter(int c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

static int is_digit(int c) {
	return c >= '0' && c <= '9';
}

static int str_equals(const char *a, const char *b) {
	while (*a && *b && *a == *b) {
		++a;
		++b;
	}
	return *a == '\0' && *b == '\0';
}

static int is_keyword(const char *lexeme) {
	return str_equals(lexeme, "int") || str_equals(lexeme, "float") ||
		   str_equals(lexeme, "char") || str_equals(lexeme, "if") ||
		   str_equals(lexeme, "else") || str_equals(lexeme, "while") ||
		   str_equals(lexeme, "return");
}

static void emit_token(const char *type, const char *lexeme) {
	printf("<%s, %s>\n", type, lexeme);
}

static void scan_identifier_or_keyword(int first_char) {
	char buf[MAX_TOKEN_LEN];
	int len = 0;
	int c = first_char;

	while (1) {
		if (len < MAX_TOKEN_LEN - 1) {
			buf[len++] = (char)c;
		}

		int next = getchar();
		if (next == EOF || !(is_letter(next) || is_digit(next) || next == '_')) {
			if (next != EOF) {
				ungetc(next, stdin);
			}
			break;
		}
		c = next;
	}

	buf[len] = '\0';
	if (is_keyword(buf)) {
		emit_token("KEYWORD", buf);
	} else {
		emit_token("IDENTIFIER", buf);
	}
}

static void scan_number(int first_char) {
	char buf[MAX_TOKEN_LEN];
	int len = 0;
	int c = first_char;

	while (1) {
		if (len < MAX_TOKEN_LEN - 1) {
			buf[len++] = (char)c;
		}

		int next = getchar();
		if (next == EOF || !is_digit(next)) {
			if (next != EOF) {
				ungetc(next, stdin);
			}
			break;
		}
		c = next;
	}

	buf[len] = '\0';
	emit_token("NUMBER", buf);
}

static int is_operator_char(int c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '=';
}

static int is_punctuation_char(int c) {
	return c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',';
}

int main(void) {
	int ch;

	while ((ch = getchar()) != EOF) {
		if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f' || ch == '\v') {
			continue; // skip all whitespace
		}

		if (is_letter(ch)) {
			scan_identifier_or_keyword(ch);
			continue;
		}

		if (is_digit(ch)) {
			scan_number(ch);
			continue;
		}

		if (is_operator_char(ch)) {
			char op[2];
			op[0] = (char)ch;
			op[1] = '\0';
			emit_token("OPERATOR", op);
			continue;
		}

		if (is_punctuation_char(ch)) {
			char p[2];
			p[0] = (char)ch;
			p[1] = '\0';
			emit_token("PUNCTUATION", p);
			continue;
		}

		// Anything else is treated as UNKNOWN.
		char unk[2];
		unk[0] = (char)ch;
		unk[1] = '\0';
		emit_token("UNKNOWN", unk);
	}

	return 0;
}
