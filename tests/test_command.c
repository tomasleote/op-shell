/* Command tests: construction, option accumulation, list linking, teardown. */
#include "../command.h"
#include "test.h"
#include "tests.h"

void run_command_tests(void) {
  SUITE("command");

  {
    Command *c = createCommand("ls");
    CHECK_STR_EQ(c->command, "ls", "createCommand copies the name");
    CHECK_INT_EQ(c->type, CMD_EXTERNAL, "default type is external");
    CHECK_INT_EQ(c->optionCount, 0, "no options at creation");
    CHECK(c->options == NULL, "options start NULL");
    CHECK(c->commandPath == NULL, "commandPath starts NULL");
    CHECK(c->next == NULL && c->previous == NULL, "links start NULL");
    freeCommand(c);
  }

  {
    Command *c = createCommand("ls");
    addOptionToCommand(c, "-l");
    addOptionToCommand(c, "-a");
    CHECK_INT_EQ(c->optionCount, 2, "two options accumulated");
    CHECK_STR_EQ(c->options[0], "-l", "first option stored");
    CHECK_STR_EQ(c->options[1], "-a", "second option stored");
    freeCommand(c);
  }

  {
    Command *head = createCommand("a");
    Command *second = createCommand("b");
    appendCommand(&head, second);
    CHECK(head->next == second, "append links head to second");
    CHECK(second->previous == head, "append sets back-link");
    CHECK(second->next == NULL, "tail next is NULL");
    freeCommand(second);
    freeCommand(head);
  }

  {
    /* appendCommand into an empty list sets the head. */
    Command *head = NULL;
    Command *only = createCommand("solo");
    appendCommand(&head, only);
    CHECK(head == only, "append to empty list sets head");
    freeCommand(only);
  }
}
