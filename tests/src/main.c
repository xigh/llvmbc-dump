extern int foo();
extern int bar();

int main(int argc, char **argv) {
    return foo() + bar() + argc;
}
