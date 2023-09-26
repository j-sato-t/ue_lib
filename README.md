# ue_lib

# 機能

![](out/class/class.png)  
拡大は元画像（out/class/class.png）を使ってください

## Manageable
- 状態を管理できるクラス
  - 生成と処理の開始を明示的に分けることができる
  - 時間のかかる処理を待ってから処理の開始をすることが可能
### 基本的な使い方
1. 継承するクラスを作成し、各関数をオーバーライド
    - `OnOpen` : 初期化処理を実装する
      - 実行に必要な準備を非同期で開始する場合はそれを登録する
    - `OnReady` : 準備が完了し、動作が開始する直前の処理を実装する
      - 非同期での準備がない場合は OnOpen のあと直ぐに呼ばれる
    - `OnPause` : 一時停止する際の処理を実装する
    - `OnResume` : 一時停止から復帰する際の処理を実装する
    - `OnClose` : 終了時の後処理を実装する
    - コードで実装する場合は `_Implementation` がついた関数をオーバーライドする
2. オブジェクトを生成し、 `Open` を実行する
3. 終了時は `Close` を呼ぶ

![](out/class/manageable_open.png)  
![](out/class/manageable_close.png)

### WaitableBase
- 処理が終わったかどうかを命じさせるクラス
  - 処理の開始時に `StartTask` が呼ばれる
    - コードの場合は `StartTask_Implementation` 
  - 処理が成功したら `SetFinish` を呼ぶ
    - 失敗時は `SetFailed`
## Logger
- ログ機能のラップ
- ログレベルや時刻を入れたログを出力する
  - 特定レベル未満のログをフィルタリングする機能あり
- `LogInfo` 等でログレベルに応じたログを出力できる

# 不具合等
## Manageable
- [ ] ログのタグのデフォルトがクラス名を入れているが継承先でも基底の `UManageable` になる
- [ ] Running状態以外でもTickを止められない
  - これに関してはそのままでも問題ない？ 
