 #ifndef _MY_MOCK_HPP
 #define _MY_MOCK_HPP

#include <sql/mysqld.h>
#include <sql/sql_class.h>
#include <sql/sql_lex.h>
#include <sql/sql_parse.h>
#include <my_inttypes.h>
#include <sql/error_handler.h>

 /**
   A mock error handler which registers itself with the THD in the CTOR,
   and unregisters in the DTOR. The function handle_condition() will
   verify that it is called with the expected error number.
   The DTOR will verify that handle_condition() has actually been called.
*/
class Mock_error_handler : public Internal_error_handler {
 public:
  Mock_error_handler(THD *thd, uint expected_error);
  ~Mock_error_handler() override;

  bool handle_condition(THD *thd, uint sql_errno, const char *sqlstate,
                        Sql_condition::enum_severity_level *level,
                        const char *msg) override;

  int handle_called() const { return m_handle_called; }

 private:
  THD *m_thd;
  uint m_expected_error;
  int m_handle_called;
};

#endif