# <h1 align="center">🚀 Space Forces — 3D シューティング / エンジンプロジェクト</h1>

<p align="center">
&nbsp; <b>C++・DirectX 11・3Dシューティング / 個人制作プロジェクト</b><br>
&nbsp; 高速シューティング × シーン管理 × シェーダ制御
</p>

<p align="center">
<img src="https://img.shields.io/badge/Engine-DirectX11-000000?style=for-the-badge"> 
<img src="https://img.shields.io/badge/Language-C++-239120?logo=c%2B%2B&style=for-the-badge">
<img src="https://img.shields.io/badge/Platform-PC-blue?style=for-the-badge">
<img src="https://img.shields.io/badge/Status-Active-success?style=for-the-badge">
</p>

---

## 🎮 プロジェクト概要 / Project Overview

Space Forcesは、プレイヤーが宇宙船を操作して敵や障害物を回避しながら戦う  
3Dシューティングゲーム/エンジン学習プロジェクトです。  

DirectX 11を使用し、レンダリング、シェーダ、入力管理、物理計算を組み合わせ、  
シンプルながらレスポンスの良いゲーム体験を目指しています。

---

## 🧩 主な特徴 / Key Features

### ✈️ 宇宙船操作
- 前進・後退・旋回・ブースト操作  
- 状態管理による滑らかな動作遷移  

### 💥 敵・障害物システム
- スポーンポイントに基づく自動生成  
- 難易度に応じた出現頻度の調整  

### 🎨 シェーダ・レンダリング
- HLSLシェーダによる基本ライティング  
- マテリアル・テクスチャの動的制御  

### ⚙️ 最適化
- 不要オブジェクトの非アクティブ化  
- 軽量なゲームループ設計  

---

## 🛠 使用技術 / Tech Stack

| 分類 | 内容 |
|------|------|
| ゲームエンジン | DirectX 11 |
| 言語 | C++ |
| シェーダ | HLSL |
| レンダリング | カスタムレンダラー |
| 入力 | キーボード / マウス |
| バージョン管理 | Git / GitHub |

---

## 📂 プロジェクト構造 / Directory Structure
SpaceForces/
├── source/
│ ├── public/ # 公開ヘッダ & 共通クラス
│ ├── private/ # 内部実装
├── data/
└── README.md


---

## 🚀 実行方法 / How to Run

### 🔹 方法①：Visual Studio でビルド
```bash
git clone https://github.com/HekSeon/space-forces.git
cd space-forces
# Visual Studioでプロジェクトを開き、ビルド
```
必要なVisual Studioバージョン: 2019以降

プロジェクト内のAdditional Include Directoriesを確認してください

🔹 方法②：ビルド済みバイナリ（後日追加予定）

GitHub Releasesにアップロードされる.exeを実行

🧠 学んだこと / What I Learned

DirectX 11によるレンダリングパイプラインの構築

HLSLシェーダの作成とマテリアル管理

ゲームループ・入力管理・状態遷移の設計

ディレクトリ構造とプロジェクト管理の重要性

🔮 今後のアップデート / Future Improvement

敵AIの追加

スコア / UI システムの実装

パーティクル・エフェクト強化

マルチプラットフォーム対応

👤 作者 / Author

BAYAR SEMIH (HekSeon)
HAL東京ゲームプログラマー学科 / 新卒ゲームプログラマー
DirectX11・Unity・Unreal によるゲーム開発を学習中

