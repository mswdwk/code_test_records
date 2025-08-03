// 声明子模块（文件会按这里声明的顺序被编译）
pub mod market_data;
pub mod strategy;
pub mod order;
pub mod risk_management;
pub mod backtest;

// 可选：重新导出常用类型（简化外部调用）
pub use market_data::{MarketData, StockData};
pub use strategy::BeijingTraderModel;
pub use order::{Order, OrderType, OrderStatus};