package com.example;

import org.apache.calcite.sql.fun.SqlStdOperatorTable;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;

import org.apache.calcite.sql.SqlNode;
import org.apache.calcite.sql.parser.SqlParseException;
import org.apache.calcite.sql.parser.SqlParser;
import org.apache.calcite.sql.util.SqlOperatorTables;
import org.apache.calcite.sql.validate.SqlValidator;
import org.apache.calcite.sql.validate.SqlValidatorImpl;
import org.apache.calcite.sql.validate.SqlValidatorUtil;

import java.util.List;

import static org.apache.calcite.sql.validate.SqlValidatorUtil.*;

@SpringBootTest
class SpringBootR2dbcMysqlExampleApplicationTests {

	@Test
	void contextLoads() {
	}

	/*public static String calculateFingerprint(String sql) throws SqlParseException {
		SqlParser.Config config = SqlParser.configBuilder()
				.setCaseSensitive(false)
				.build();
		SqlParser parser = SqlParser.create(sql, config);
		SqlNode sqlNode = parser.parseQuery();

		// 使用默认的操作表
		SqlValidator.Config validatorConfig = SqlValidator.Config.DEFAULT
				.withDefaultOperatorTable(SqlStdOperatorTable.instance());
		SqlValidator validator = new SqlValidatorImpl(validatorConfig);

		// 验证并准备SQL节点
		validator.validate(sqlNode);

		// 计算指纹
		List<String> digests = SqlValidatorUtil.getDigests(sqlNode);
		return String.join(",", digests);
	}*/
	@Test
	void sqlDigest(){
		try {
			String sql = "SELECT * FROM employees WHERE employee_id = 100";
			String fingerprint = "";//calculateFingerprint(sql);
			System.out.println("SQL Fingerprint: " + fingerprint);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
