// market_data.rs
use chrono::{DateTime, Local};
use serde::{Serialize, Deserialize};
use std::collections::HashMap;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct StockData {
    pub symbol: String,       // 股票代码，如 "600000"
    pub name: String,         // 股票名称
    pub open: f64,           // 开盘价
    pub high: f64,           // 最高价
    pub low: f64,            // 最低价
    pub close: f64,          // 收盘价
    pub volume: f64,         // 成交量(手)
    pub amount: f64,         // 成交额(万元)
    pub timestamp: DateTime<Local>, // 时间戳
}

#[derive(Debug)]
pub struct MarketData {
    pub stocks: HashMap<String, StockData>, // 股票代码到数据的映射
}

impl MarketData {
    pub fn new() -> Self {
        MarketData {
            stocks: HashMap::new(),
        }
    }
    
    // 更新股票数据
    pub fn update(&mut self, data: StockData) {
        self.stocks.insert(data.symbol.clone(), data);
    }
    
    // 获取股票数据
    pub fn get(&self, symbol: &str) -> Option<&StockData> {
        self.stocks.get(symbol)
    }
}