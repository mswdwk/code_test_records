// strategy.rs
use super::{market_data::StockData, order::Order};

pub struct BeijingTraderModel {
    // 策略参数
    pub short_term_period: usize,   // 短线周期
    pub volume_multiplier: f64,    // 成交量倍数阈值
    pub stop_loss_pct: f64,        // 止损百分比
    pub take_profit_pct: f64,      // 止盈百分比
    pub position_ratio: f64,       // 仓位比例
}

impl BeijingTraderModel {
    pub fn new() -> Self {
        BeijingTraderModel {
            short_term_period: 5,
            volume_multiplier: 1.5,
            stop_loss_pct: 0.05,
            take_profit_pct: 0.08,
            position_ratio: 0.3,
        }
    }
    
    // 生成交易信号
    pub fn generate_signal(&self, stock_data: &StockData, history_data: &[StockData]) -> Option<Order> {
        if history_data.len() < self.short_term_period {
            return None;
        }
        
        // 计算短期均线
        let short_ma: f64 = history_data.iter()
            .rev()
            .take(self.short_term_period)
            .map(|d| d.close)
            .sum::<f64>() / self.short_term_period as f64;
        
        // 北京炒家模型核心逻辑
        // 1. 放量突破短期均线买入
        // 2. 跌破短期均线或达到止盈止损卖出
        
        let is_volume_ok = stock_data.volume > history_data.iter()
            .rev()
            .take(self.short_term_period)
            .map(|d| d.volume)
            .sum::<f64>() / self.short_term_period as f64 * self.volume_multiplier;
        
        let is_break_up = stock_data.close > short_ma && stock_data.open < short_ma;
        
        if is_break_up && is_volume_ok {
            // 买入信号
            Some(Order::new(
                stock_data.symbol.clone(),
                stock_data.close,
                (self.position_ratio * 10000.0) as u32, // 假设账户有1万元，按比例计算股数
                OrderType::Buy,
            ))
        } else if stock_data.close < short_ma {
            // 卖出信号
            Some(Order::new(
                stock_data.symbol.clone(),
                stock_data.close,
                0, // 数量由持仓决定，这里只是信号
                OrderType::Sell,
            ))
        } else {
            None
        }
    }
}