# [Robot Framework官方教程（六）变量](https://www.jianshu.com/p/de16a15d21bc)

## 变量简介
变量是Robot Framework的一个不可或缺的特性，它们可以在测试数据的大多数地方使用。最常见的是，它们用于测试用例表和关键字表中关键字的参数，但所有设置都允许在其值中使用变量。无法使用变量指定普通关键字名称，但可以使用BuiltIn关键字 Run Keyword获得相同的效果。

Robot Framework本身有两种变量，scalars和lists，它们分别具有$ {SCALAR}和@ {LIST}的语法。除此之外，环境变量可以直接与语法％{VARIABLE}一起使用。

在以下情况下建议使用变量：

当字符串经常在测试数据中更改时。使用变量，您只需在一个地方进行这些更改。
创建独立于系统和操作系统的测试数据时。使用变量而不是硬编码字符串可以大大减轻。因为在启动测试时可以从命令行设置变量，所以更改系统特定的变量很容易。这也有助于本地化测试，这通常涉及使用不同的字符串运行相同的测试。
当需要将字符串以外的对象作为关键字的参数时。
当不同的关键字，甚至在不同的测试库中，需要进行通信。您可以将一个关键字的返回值分配给变量，并将其作为参数提供给另一个变量。
当测试数据中的值很长或者复杂时。例如，$ {URL}比http://long.domain.name:8080/path/to/service?foo=1&bar=2&zap=42短 。
如果在测试数据中使用了不存在的变量，则使用它的关键字将失败。如果需要使用与变量相同的语法作为文字字符串，则必须使用反斜杠对其进行转义，如\ $ {NAME}。

