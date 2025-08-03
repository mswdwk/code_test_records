// order.rs
use chrono::{DateTime, Local};
use serde::{Serialize, Deserialize};

#[derive(Debug, Clone, Serialize, Deserialize, PartialEq)]
pub enum OrderType {
    Buy,    // 买入
    Sell,   // 卖出
    Cancel, // 撤单
}

#[derive(Debug, Clone, Serialize, Deserialize, PartialEq)]
pub enum OrderStatus {
    Pending,    // 待成交
    Filled,     // 已成交
    PartFilled, // 部分成交
    Cancelled,   // 已取消
    Rejected,    // 已拒绝
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Order {
    pub order_id: String,        // 订单ID
    pub symbol: String,          // 股票代码
    pub price: f64,             // 价格
    pub quantity: u32,          // 数量(股)
    pub order_type: OrderType,  // 订单类型
    pub status: OrderStatus,    // 订单状态
    pub timestamp: DateTime<Local>, // 创建时间
}

impl Order {
    pub fn new(
        symbol: String,
        price: f64,
        quantity: u32,
        order_type: OrderType,
    ) -> Self {
        Order {
            order_id: uuid::Uuid::new_v4().to_string(),
            symbol,
            price,
            quantity,
            order_type,
            status: OrderStatus::Pending,
            timestamp: Local::now(),
        }
    }
}