# ShopFilter — AI 驱动的智能选品助手

<p align="center">
  <img src="https://img.shields.io/badge/Qt-6-41CD52?style=flat-square&logo=qt&logoColor=white" />
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus&logoColor=white" />
  <img src="https://img.shields.io/badge/CMake-3.16-064F8C?style=flat-square&logo=cmake&logoColor=white" />
  <img src="https://img.shields.io/badge/OpenCode-AI%20Agent-3B82F6?style=flat-square&logo=robot&logoColor=white" />
  <img src="https://img.shields.io/badge/License-MIT-green?style=flat-square" />
</p>

> **本项目全程由 OpenCode AI 智能体辅助开发完成，从架构设计到代码实现，人机协作打造。**

---

ShopFilter 是一个智能化的电商商品聚合与筛选桌面应用，模拟"购物智能体"的思路：

<p align="center">
  <b>输入意图 → 多源搜索 → 智能过滤 → 综合排序 → 输出最优决策</b>
</p>

基于 **Qt6 + C++17** 构建，你只需告诉它要什么，剩下的搜索、比对、排序、筛选全自动完成。

---

## 功能

| 模块 | 能力 |
|------|------|
| 意图理解 | 关键词输入，智能体自动多平台并发抓取 |
| 多维感知 | 解析价格、评分、销量、折扣、包邮、库存等信息 |
| 智能决策 | 内置加权综合评分模型，自动计算商品性价比 |
| 策略筛选 | 可配置筛选规则（价格区间、最低评分、免邮等） |
| 结果呈现 | 清洗去噪，按最优策略排序输出 |

---

## 构建

```bash
# 依赖: Qt6 (Widgets), CMake >= 3.16, C++17
cmake -B build && cmake --build build
## 技术栈

- **语言**: C++17
- **框架**: Qt6 (Widgets + 自定义 QSS 主题)
- **架构**: 插件式多平台抓取 (IScraper 接口) + 多线程并发
- **数据**: nlohmann/json 解析
- **开发方式**: AI 智能体辅助 (OpenCode)

## 许可

MIT License
