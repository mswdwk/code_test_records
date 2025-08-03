use std::collections::HashMap;

// risk_management.rs
use super::{order::Order, market_data::StockData};

pub struct RiskManager {
    pub max_position_ratio: f64,    // 最大仓位比例
    pub max_loss_per_trade: f64,   // 单笔最大亏损
    pub max_loss_per_day: f64,     // 单日最大亏损
    pub blacklist: Vec<String>,     // 黑名单股票
}

impl RiskManager {
    pub fn new() -> Self {
        RiskManager {
            max_position_ratio: 0.5,
            max_loss_per_trade: 0.02, // 2%
            max_loss_per_day: 0.05,    // 5%
            blacklist: vec![],
        }
    }
    
    // 检查订单风险
    pub fn check_order_risk(&self, order: &Order, portfolio: &Portfolio, stock_data: &StockData) -> bool {
        // 检查黑名单
        if self.blacklist.contains(&order.symbol) {
            return false;
        }
        
        // 检查仓位比例
        let position_value = portfolio.get_position(&order.symbol).map_or(0.0, |p| p.value());
        let new_position_value = position_value + order.price * order.quantity as f64;
        
        if new_position_value > portfolio.total_value() * self.max_position_ratio {
            return false;
        }
        
        // 检查单笔亏损风险
        let potential_loss = (order.price - stock_data.low) * order.quantity as f64;
        if potential_loss > portfolio.total_value() * self.max_loss_per_trade {
            return false;
        }
        
        true
    }
}

// 简化版投资组合
pub struct Portfolio {
    pub positions: HashMap<String, Position>,
    pub cash: f64,
    pub total_value: f64,
}

impl Portfolio {
    pub fn new(initial_cash: f64) -> Self {
        Portfolio {
            positions: HashMap::new(),
            cash: initial_cash,
            total_value: initial_cash,
        }
    }
    
    pub fn total_value(&self) -> f64 {
        self.total_value
    }
    
    pub fn get_position(&self, symbol: &str) -> Option<&Position> {
        self.positions.get(symbol)
    }
}

pub struct Position {
    pub symbol: String,
    pub quantity: u32,
    pub avg_cost: f64,
}

impl Position {
    pub fn value(&self) -> f64 {
        self.quantity as f64 * self.avg_cost
    }
}