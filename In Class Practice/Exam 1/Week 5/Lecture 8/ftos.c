#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BankAccount {
  char name[50];
  int balance;

  void (*deposit)(struct BankAccount *, int);
  void (*withdraw)(struct BankAccount *, int);
  void (*display)(struct BankAccount *);
};

typedef struct BankAccount BankAccount;

void deposit(BankAccount *account, int amount) {
  account->balance += amount;
  printf("Deposited $%d. New balance: $%d\n", amount, account->balance);
}

void withdraw(BankAccount *account, int amount) {
  if (account->balance >= amount) {
    account->balance -= amount;
    printf("Withdrew $%d. New balance: $%d\n", amount, account->balance);
  } else {
    printf("Insufficient funds!\n");
  }
}

void display(BankAccount *account) {
  printf("Account: %s, Balance: $%d\n", account->name, account->balance);
}

// Constructor-like function
BankAccount create_account(const char *name, int initial_balance) {
  BankAccount account;
  strcpy(account.name, name);
  account.balance = initial_balance;

  // Assign function pointers
  account.deposit = deposit;
  account.withdraw = withdraw;
  account.display = display;

  return account;
}

int main() {
  BankAccount acc = create_account("John Doe", 1000);

  // Call methods using function pointers
  acc.display(&acc);
  acc.deposit(&acc, 500);
  acc.withdraw(&acc, 200);
  acc.display(&acc);

  return 0;
}
