/*https://www.cnblogs.com/mumulin99/p/9837432.html*/
/*
七、计算移动平均
窗口函数就是将表以窗口为单位进行分割,并在其中进行排序的函数。其实其中还包含在窗口中指定更加详细的汇总范围的备选功能,该备选功能中的汇总范围称为框架。

1、指定框架（汇总范围）
由于框架是根据当前记录来确定的,因此和固定的窗口不同,其范围会随着当前记录的变化而变化。
ROW PRECEDING 截止到之前~行
ROW FOLLOWING 截止到之后~行
*/

/*calculate avg(price)  every last 10 rows */

select trx_id,price,avg(price) over(order by trx_time ROWS 10 PRECEDING) AS prc_avg FROM trx_detail;
