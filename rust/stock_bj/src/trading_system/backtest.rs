// backtest.rs
use super::{market_data::StockData, order::Order, strategy::BeijingTraderModel, risk_management::RiskManager};
use std::collections::HashMap;

pub struct BacktestEngine {
    pub strategy: BeijingTraderModel,
    pub risk_manager: RiskManager,
    pub portfolio: Portfolio,
    pub historical_data: HashMap<String, Vec<StockData>>,
}

impl BacktestEngine {
    pub fn new(initial_cash: f64) -> Self {
        BacktestEngine {
            strategy: BeijingTraderModel::new(),
            risk_manager: RiskManager::new(),
            portfolio: Portfolio::new(initial_cash),
            historical_data: HashMap::new(),
        }
    }
    
    // 运行回测
    pub fn run(&mut self) -> BacktestResult {
        let mut result = BacktestResult::new();
        
        // 按时间顺序处理所有股票数据
        let mut all_data = Vec::new();
        for (symbol, data) in &self.historical_data {
            for d in data {
                all_data.push((d.timestamp, symbol.clone(), d.clone()));
            }
        }
        
        // 按时间排序
        all_data.sort_by(|a, b| a.0.cmp(&b.0));
        
        // 处理每一天的数据
        for (timestamp, symbol, data) in all_data {
            // 更新当前股票数据
            let history = self.historical_data.get(&symbol).unwrap();
            
            // 生成交易信号
            if let Some(order) = self.strategy.generate_signal(&data, history) {
                // 检查风险
                if self.risk_manager.check_order_risk(&order, &self.portfolio, &data) {
                    // 执行订单
                    self.execute_order(order, &data);
                }
            }
            
            // 更新投资组合价值
            self.update_portfolio_value(&data);
        }
        
        result
    }
    
    fn execute_order(&mut self, order: Order, data: &StockData) {
        // 简化版订单执行逻辑
        match order.order_type {
            OrderType::Buy => {
                let cost = order.price * order.quantity as f64;
                if self.portfolio.cash >= cost {
                    self.portfolio.cash -= cost;
                    
                    let position = self.portfolio.positions.entry(order.symbol.clone())
                        .or_insert(Position {
                            symbol: order.symbol.clone(),
                            quantity: 0,
                            avg_cost: 0.0,
                        });
                    
                    let total_quantity = position.quantity + order.quantity;
                    position.avg_cost = (position.avg_cost * position.quantity as f64 + cost) / total_quantity as f64;
                    position.quantity = total_quantity;
                }
            }
            OrderType::Sell => {
                if let Some(position) = self.portfolio.positions.get_mut(&order.symbol) {
                    let sell_quantity = order.quantity.min(position.quantity);
                    let proceeds = order.price * sell_quantity as f64;
                    
                    self.portfolio.cash += proceeds;
                    position.quantity -= sell_quantity;
                    
                    if position.quantity == 0 {
                        self.portfolio.positions.remove(&order.symbol);
                    }
                }
            }
            _ => {}
        }
    }
    
    fn update_portfolio_value(&mut self, data: &StockData) {
        let mut total = self.portfolio.cash;
        
        for (symbol, position) in &self.portfolio.positions {
            if let Some(stock_data) = self.historical_data.get(symbol)
                .and_then(|v| v.last()) {
                total += position.quantity as f64 * stock_data.close;
            }
        }
        
        self.portfolio.total_value = total;
    }
}

pub struct BacktestResult {
    pub final_value: f64,
    pub max_drawdown: f64,
    pub win_rate: f64,
    pub sharpe_ratio: f64,
}

impl BacktestResult {
    pub fn new() -> Self {
        BacktestResult {
            final_value: 0.0,
            max_drawdown: 0.0,
            win_rate: 0.0,
            sharpe_ratio: 0.0,
        }
    }
}