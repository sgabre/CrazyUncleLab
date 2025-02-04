#include <unity.h>
#include <stdint.h>
#include <Shell.h>
#include <ShellCmd.h>


static const char mHelpUsage[] = "help [command]\r\n";
static uint32_t mHelpCmd_Hander(int argc, char **argv){return 0;}

static const char mVersionUsage[] = "version [command]\r\n";
static uint32_t mVersionCmd_Hander(int argc, char **argv){return 0;}

static const char mRebootUsage[] = "reboot [command]\r\n";
static uint32_t mRebootCmd_Hander(int argc, char **argv){return 0;}

static const char mDummyUsage[] = "dummy [command]\r\n";
static uint32_t mDummyCmd_Hander(int argc, char **argv){return 0;}

static const char mDumpUsage[] = "dump [command]\r\n";
static uint32_t mDumpCmd_Hander(int argc, char **argv){return 0;}

static const char mEmptyUsage[] = "? [command]\r\n";
static uint32_t mEmptyCmd_Hander(int argc, char **argv){return 0;}

// TC #1: Invalid Parameter (aName = NULL)
void test_SHELL_ADD_CMD_1(void)
{
        
    ClearFailure();

    SHELL_ADD_CMD(NULL, mHelpUsage, mHelpCmd_Hander);
    TEST_ASSERT_EQUAL(SHELL_ERR_INVALID_PARAM, LastFailure());
}

// TC #2: Invalid Parameter (aHelp = NULL)
void test_SHELL_ADD_CMD_2(void)
{
    
    ClearFailure();
    
    SHELL_ADD_CMD("?", NULL, mHelpCmd_Hander);
    TEST_ASSERT_EQUAL(SHELL_ERR_INVALID_PARAM, LastFailure());
}

// TC #3: Invalid Parameter (aCommand = NULL)
void test_SHELL_ADD_CMD_3(void)
{
    
    ClearFailure();
    
    SHELL_ADD_CMD("?", mHelpUsage, NULL);
    TEST_ASSERT_EQUAL(SHELL_ERR_INVALID_PARAM, LastFailure());
}

// TC #4: Boundery - Upper Limit
void test_SHELL_ADD_CMD_4(void)
{
    Shell_Setup();
    
    ClearFailure();
    SHELL_ADD_CMD("?", mHelpUsage, mHelpCmd_Hander);
    TEST_ASSERT_EQUAL(SHELL_OK, LastFailure());
    
    ClearFailure();
    SHELL_ADD_CMD("ver", mVersionUsage, mVersionCmd_Hander );
    TEST_ASSERT_EQUAL(SHELL_OK, LastFailure());
    
    ClearFailure();
    SHELL_ADD_CMD("reboot", mRebootUsage, mRebootCmd_Hander );
    TEST_ASSERT_EQUAL(SHELL_OK, LastFailure());
    
    ClearFailure();
    SHELL_ADD_CMD("dump", mDumpUsage, mDumpCmd_Hander );
    TEST_ASSERT_EQUAL(SHELL_OK, LastFailure());
    
    ClearFailure();
    SHELL_ADD_CMD("dummy", mDummyUsage, mDummyCmd_Hander );
    TEST_ASSERT_EQUAL(SHELL_OK, LastFailure());
    
    ClearFailure();
    SHELL_ADD_CMD("?", mEmptyUsage, mRebootCmd_Hander );
    TEST_ASSERT_EQUAL(SHELL_ERR_MEMORY, LastFailure());    //STATIC_MEMROY
    
    //Shell_Handler();
}

void test_mShell_ParseLine_1(void)
{
    char* argv[10] = {0};
    int argc = 0;
    char input[] = "ls -l /home/user";
    //int mShell_ParseLine(char **argv, char* aStream) 
    
    //int mShell_ParseLine(NULL, input) 
    //int mShell_ParseLine(argv, NULL) 
    
    
    argc = mShell_ParseLine(argv, input);
    TEST_ASSERT_EQUAL_STRING("ls", argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", argv[1]);
    TEST_ASSERT_EQUAL_STRING("/home/user", argv[2]);
    TEST_ASSERT_EQUAL(3,argc);
}

void test_mShell_ParseLine_3(void)
{
    char* argv[10] = {0};
    int argc = 0;
    char input[] = "ls -l /home/user";
    //int mShell_ParseLine(char **argv, char* aStream) 
    
    //int mShell_ParseLine(argv, NULL) 
    //int mShell_ParseLine(NULL, " ") 
    
    argc = mShell_ParseLine(argv, input);
    TEST_ASSERT_EQUAL_STRING("ls", argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", argv[1]);
    TEST_ASSERT_EQUAL_STRING("/home/user", argv[2]);
    TEST_ASSERT_EQUAL(3,argc);
}

void test_mShell_ParseLine_4(void)
{
    char* argv[10] = {0};
    int argc = 0;
    char input[] = "ls -l /home/user &";
    //int mShell_ParseLine(char **argv, char* aStream) 
    
    //int mShell_ParseLine(argv, NULL) 
    //int mShell_ParseLine(NULL, " ") 
    
    argc = mShell_ParseLine(argv, input);
    TEST_ASSERT_EQUAL_STRING("ls", argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", argv[1]);
    TEST_ASSERT_EQUAL_STRING("/home/user", argv[2]);
    TEST_ASSERT_EQUAL_STRING("&", argv[3]);
    TEST_ASSERT_EQUAL(4,argc);
}

void test_mShell_ParseLine_5(void)
{
    char* argv[10] = {0};
    int argc = 0;
    char input[] = "ls -l /home/user &(10)";
    //int mShell_ParseLine(char **argv, char* aStream) 
    
    //int mShell_ParseLine(argv, NULL) 
    //int mShell_ParseLine(NULL, " ") 
    
    argc = mShell_ParseLine(argv, input);
    TEST_ASSERT_EQUAL_STRING("ls", argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", argv[1]);
    TEST_ASSERT_EQUAL_STRING("/home/user", argv[2]);
    TEST_ASSERT_EQUAL_STRING("&(10)", argv[3]);
    TEST_ASSERT_EQUAL(4,argc);
}

// Check, if the last but one entry is a ">" or ">>" - in this case, we have to redirect the output to a file instead of stdout
void test_mShell_ParseLine_6(void)
{
    char* argv[10] = {0};
    int argc = 0;
    char input[] = "ls -l /home/user > list.txt";
    //int mShell_ParseLine(char **argv, char* aStream) 
    
    //int mShell_ParseLine(argv, NULL) 
    //int mShell_ParseLine(NULL, " ") 
    
    argc = mShell_ParseLine(argv, input);
    TEST_ASSERT_EQUAL_STRING("ls", argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", argv[1]);
    TEST_ASSERT_EQUAL_STRING("/home/user", argv[2]);
    TEST_ASSERT_EQUAL_STRING(">", argv[3]);
    TEST_ASSERT_EQUAL_STRING("list.txt", argv[4]);
    TEST_ASSERT_EQUAL(5,argc);
}

void test_mShell_ParseLine_7(void)
{
    char* argv[10] = {0};
    int argc = 0;
    char input[] = "ls -l /home/user >> list.txt";
    //int mShell_ParseLine(char **argv, char* aStream) 
    
    //int mShell_ParseLine(argv, NULL) 
    //int mShell_ParseLine(NULL, " ") 
    
    argc = mShell_ParseLine(argv, input);
    TEST_ASSERT_EQUAL_STRING("ls", argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", argv[1]);
    TEST_ASSERT_EQUAL_STRING("/home/user", argv[2]);
    TEST_ASSERT_EQUAL_STRING(">>", argv[3]);
    TEST_ASSERT_EQUAL_STRING("list.txt", argv[4]);
    TEST_ASSERT_EQUAL(5,argc);
}

void test_mShell_ParseLine_8(void)
{
    char* argv[10] = {0};
    int argc = 0;
    char input[] = "ls -l /home/user >> list.txt &";
    //int mShell_ParseLine(char **argv, char* aStream) 
    
    //int mShell_ParseLine(argv, NULL) 
    //int mShell_ParseLine(NULL, " ") 
    
    argc = mShell_ParseLine(argv, input);
    TEST_ASSERT_EQUAL_STRING("ls", argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", argv[1]);
    TEST_ASSERT_EQUAL_STRING("/home/user", argv[2]);
    TEST_ASSERT_EQUAL_STRING(">>", argv[3]);
    TEST_ASSERT_EQUAL_STRING("list.txt", argv[4]);
    TEST_ASSERT_EQUAL_STRING("&", argv[5]);
    TEST_ASSERT_EQUAL(6,argc);
}

void test_mShell_ParseLine_9(void)
{
    char* argv[10] = {0};
    int argc = 0;
    char input[] = "ls -l /home/user >> list.txt &(10)";
    //int mShell_ParseLine(char **argv, char* aStream) 
    
    //int mShell_ParseLine(argv, NULL) 
    //int mShell_ParseLine(NULL, " ") 
    
    argc = mShell_ParseLine(argv, input);
    TEST_ASSERT_EQUAL_STRING("ls", argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", argv[1]);
    TEST_ASSERT_EQUAL_STRING("/home/user", argv[2]);
    TEST_ASSERT_EQUAL_STRING(">>", argv[3]);
    TEST_ASSERT_EQUAL_STRING("list.txt", argv[4]);
    TEST_ASSERT_EQUAL_STRING("&(10)", argv[5]);
    TEST_ASSERT_EQUAL(6,argc);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_SHELL_ADD_CMD_1);
    RUN_TEST(test_SHELL_ADD_CMD_2);
    RUN_TEST(test_SHELL_ADD_CMD_3);
    RUN_TEST(test_SHELL_ADD_CMD_4);
    RUN_TEST(test_mShell_ParseLine_3);
    RUN_TEST(test_mShell_ParseLine_4);
    RUN_TEST(test_mShell_ParseLine_5);
    RUN_TEST(test_mShell_ParseLine_6);
    RUN_TEST(test_mShell_ParseLine_7);
    RUN_TEST(test_mShell_ParseLine_8);
    RUN_TEST(test_mShell_ParseLine_9);
        
    return UNITY_END();
}