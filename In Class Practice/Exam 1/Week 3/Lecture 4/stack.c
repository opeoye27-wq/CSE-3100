#include <stdio.h>
int add(int a, int b) { // Code lives in TEXT segment (permanent)
int result = a + b; // 'result' lives on STACK (temporary)
return result;
}
int main() {
int x = 5; // 'x' on stack
int y = 10; // 'y' on stack
int sum = add(x, y); // Creates stack frame for add()
// Stack frame destroyed after return
printf("Sum: %d\n", sum);
return 0;
}
```
## Memory During Execution
### Before calling `add()`:
```
TEXT SEGMENT (loaded once at startup, never changes):
┌─────────────────────┐
│ main() instructions │ ← Code for main()
│ add() instructions │ ← Code for add() (already here!)
└─────────────────────┘
STACK (changes during execution):
┌─────────────────┐
│ x = 5 │
│ y = 10 │
│ sum = ? │
└─────────────────┘
```
### During `add()` execution:
```
TEXT SEGMENT (unchanged):
┌─────────────────────┐
│ main() instructions │
│ add() instructions │ ← CPU executes these instructions
└─────────────────────┘
STACK (new frame added):
┌─────────────────┐
│ a = 5 │ ← add()'s parameters
│ b = 10 │
│ result = 15 │ ← add()'s local variable
│ return address │ ← where to go back
├─────────────────┤
│ x = 5 │
│ y = 10 │
│ sum = ? │
└─────────────────┘
```
### After `add()` returns:
```
TEXT SEGMENT (still unchanged):
┌─────────────────────┐
│ main() instructions │
│ add() instructions │ ← Still here! Never removed
└─────────────────────┘
STACK (frame removed):
┌─────────────────┐
│ x = 5 │
│ y = 10 │
│ sum = 15 │ ← Return value stored
└─────────────────┘