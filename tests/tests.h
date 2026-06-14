#ifndef TESTS_H
#define TESTS_H

/* Each suite lives in its own file and is run from test_main.c. */
void run_scanner_tests(void);
void run_parser_tests(void);
void run_command_tests(void);
void run_builtins_tests(void);

#endif /* TESTS_H */
