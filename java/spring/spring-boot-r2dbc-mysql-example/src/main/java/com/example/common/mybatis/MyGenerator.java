package com.example.common.mybatis;

import cn.hutool.json.JSONUtil;
import org.mybatis.generator.api.IntrospectedColumn;
import org.mybatis.generator.api.IntrospectedTable;
import org.mybatis.generator.api.PluginAdapter;
import org.mybatis.generator.api.dom.java.*;
import org.mybatis.generator.internal.util.StringUtility;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedHashMap;
import java.util.List;

/**
 * 字段和类生成时，会调用PluginAdapter中的方法，所以可以按需求重写对应的方法
 */
public class MyGenerator extends PluginAdapter {

    private static final String SEPARATION = ",";

    private static final String JSON_PREFIX = "{";

    private static final String JSON_SUFFIX = "}";

    private boolean makeConstant = true;

    @Override
    public boolean validate(List<String> list) {
        return true;
    }

    /**
     * 每个字段不生成get方法
     */
    @Override
    public boolean modelGetterMethodGenerated(Method method,
                                              TopLevelClass topLevelClass,
                                              IntrospectedColumn introspectedColumn,
                                              IntrospectedTable introspectedTable,
                                              ModelClassType modelClassType) {
        return false;
    }

    /**
     * 每个字段不生成set方法
     */
    @Override
    public boolean modelSetterMethodGenerated(Method method,
                                              TopLevelClass topLevelClass,
                                              IntrospectedColumn introspectedColumn,
                                              IntrospectedTable introspectedTable,
                                              ModelClassType modelClassType) {
        return false;
    }

    /**
     * 类上增加对应的注解，并生成类注释
     */
    @Override
    public boolean modelBaseRecordClassGenerated(TopLevelClass topLevelClass,
                                                 IntrospectedTable introspectedTable) {
        // 到入需要注解的包
        topLevelClass.addImportedType("lombok.Data");
        topLevelClass.addImportedType("lombok.Builder");
        topLevelClass.addImportedType("lombok.NoArgsConstructor");
        topLevelClass.addImportedType("lombok.AllArgsConstructor");

        // 添加domain的注解
        topLevelClass.addAnnotation("@Data");
        topLevelClass.addAnnotation("@Builder");
        topLevelClass.addAnnotation("@NoArgsConstructor");
        topLevelClass.addAnnotation("@AllArgsConstructor");

        // 添加domain的注释
        topLevelClass.addJavaDocLine("/**");
        topLevelClass.addJavaDocLine("* @ClassName: " + topLevelClass.getType().getShortName());
        topLevelClass.addJavaDocLine("* @Description: ");
        topLevelClass.addJavaDocLine("* @author: Mybatis Generator");
        topLevelClass.addJavaDocLine("* @date " + date2Str(new Date()));
        topLevelClass.addJavaDocLine("*/");
        return true;
    }

    /**
     * 生成属性字段注释（将数据库字段描述作为属性注释）
     */
    @Override
    public boolean modelFieldGenerated(Field field,
                                       TopLevelClass topLevelClass,
                                       IntrospectedColumn introspectedColumn,
                                       IntrospectedTable introspectedTable,
                                       ModelClassType modelClassType) {
        this.comment(field, topLevelClass, introspectedColumn, introspectedTable);
        return true;
    }

    private void comment(JavaElement element,
                         TopLevelClass topLevelClass,
                         IntrospectedColumn introspectedColumn,
                         IntrospectedTable introspectedTable) {
        element.getJavaDocLines().clear();
        element.addJavaDocLine("/**");
        //获取列字段注解
        String remark = introspectedColumn.getRemarks();
        if (remark != null && remark.length() > 1) {
            element.addJavaDocLine(" * " + remark);
        }

        // mysql 中自增主键，添加 AutoIncrement:  true 说明
        boolean autoIncrement = introspectedColumn.isAutoIncrement();
        if (autoIncrement) {
            element.addJavaDocLine(" * AutoIncrement:  true");
        }
        element.addJavaDocLine(" */");
        if (this.makeConstant) {
            System.out.println("start constant " + introspectedColumn.getActualColumnName());
            if (StringUtility.stringHasValue(remark) && remark.contains(JSON_PREFIX) && remark.contains(JSON_SUFFIX)) {
                //截取常量字符串
                String commentJson = remark.substring(remark.indexOf(JSON_PREFIX), remark.lastIndexOf(JSON_SUFFIX) + 1);
                try {
                    LinkedHashMap<String, String> commentMap = JSONUtil.toBean(commentJson, LinkedHashMap.class);

                    commentMap.forEach((key, value) -> {
                        //常量字段属性名 以列字段名+_+json key 值为字段名
                        String name = introspectedColumn.getActualColumnName().toUpperCase() + "_" + key.toUpperCase();
                        //设置常量字段类型与列字段一致
                        Field field = new Field(name, introspectedColumn.getFullyQualifiedJavaType());
                        field.setStatic(true);
                        field.setFinal(true);
                        //常量字段属性描述
                        String desc = "";
                        //常量字段属性值
                        String constant = "";
                        if (value.contains(SEPARATION)) {
                            String[] split = value.split(SEPARATION);
                            constant = split[0];
                            desc = split[1];
                        } else {
                            constant = value;
                        }
                        field.setInitializationString(constant);
                        field.setVisibility(JavaVisibility.PUBLIC);
                        field.addJavaDocLine("/**");
                        field.addJavaDocLine("* " + introspectedColumn.getActualColumnName() + ":" + desc);
                        field.addJavaDocLine("*/");
                        if (introspectedTable.getTargetRuntime() == IntrospectedTable.TargetRuntime.MYBATIS3_DSQL) {
                            context.getCommentGenerator().addFieldAnnotation(field, introspectedTable,
                                    topLevelClass.getImportedTypes());
                        } else {
                            context.getCommentGenerator().addFieldComment(field, introspectedTable);
                        }
                        //将常量加入BD实体类
                        topLevelClass.addField(field);
                    });
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private String date2Str(Date date) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        return sdf.format(date);
    }
}