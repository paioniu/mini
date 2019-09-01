

#include <mini/test.h>
#include <mini/base/type.h>
#include <mini/base/error.h>

TEST_MODULE

Private int base_error_c__test_all_aux001() {
    SHIELD(0, "Falha falha", return 100;)
    return 0;
}

TEST_IMPL_TEST(base_error_c__test_all) {
    TEST_ASSERT(ErrorGetStatus() == 0, "Status deveria ser 0 no início")
    ErrorSetStatus(10);
    TEST_ASSERT(ErrorGetStatus() == 10, "Status deveria ser 10 após ErrorSetStatus(10)")
    ErrorResetStatus();
    TEST_ASSERT(ErrorGetStatus() == 0, "Status deveria ser 0 após ErrorResetStatus")

    ErrorResetStatus();
    int res = base_error_c__test_all_aux001();
    TEST_ASSERT(ErrorGetStatus() == 1, "Status deveria ser 1 após um SHIELD")
    TEST_ASSERT(res == 100, "Resultado deveria ser o retornado pelo SHIELD")

    TEST_ASSERT(ErrorGetLogStatus() == 1, "Log Status deveria ser 1 no início")
    ErrorDisableLog();
    TEST_ASSERT(ErrorGetLogStatus() == 0, "Log Status deveria ser 0 após ErrorDisableLog()")
    ErrorEnableLog();
    TEST_ASSERT(ErrorGetLogStatus() == 1, "Log Status deveria ser 1 após ErrorEnableLog()")
    ErrorToggleLog();
    TEST_ASSERT(ErrorGetLogStatus() == 0, "Log Status deveria ser 0 após ErrorToggleLog()")


    TEST_ASSERT(ErrorGetPrintstackStatus() == 0, "Printstack Status deveria ser 0 no início")
    ErrorEnablePrintstack();
    TEST_ASSERT(ErrorGetPrintstackStatus() == 1, "Printstack Status deveria ser 1 após ErrorEnablePrintstack()")
    ErrorDisablePrintstack();
    TEST_ASSERT(ErrorGetPrintstackStatus() == 0, "Printstack Status deveria ser 0 após ErrorDisablePrintstack()")
    ErrorTogglePrintstack();
    TEST_ASSERT(ErrorGetPrintstackStatus() == 1, "Printstack Status deveria ser 1 após ErrorToggleLog()")

    ErrorResetStatus();
    base_error_c__test_all_aux001();
}

TEST_IMPL_TEST(base_error_c__tests) {
    TEST_CALL_TEST(base_error_c__test_all)
}
