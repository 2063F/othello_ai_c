#ifndef SEARCH_H
#define SEARCH_H

#include "othello.h"

typedef struct {
    int square;        /* 選んだ手のマス番号。-1 はパス（合法手が無い場合） */
    double score;       /* 手番側から見た評価値（中盤=評価関数値、終盤=確定石差） */
    long nodes;         /* 探索したノード数（デバッグ・速度確認用） */
    int depth_reached;  /* 反復深化で実際に完了した深さ */
} SearchResult;

/*
 * 中盤〜序盤探索: 反復深化 + negamax + αβ枝刈り + 置換表。
 * time_limit_ms ミリ秒以内で探索を打ち切り、その時点で分かっている最善手を返す。
 * max_depth は深さの上限（安全弁。時間切れが起きなければここまで読む）。
 * 残り空きマス数が endgame_threshold 以下になったら自動的に終盤完全読みに切り替える。
 */
SearchResult find_best_move(Bitboard black, Bitboard white, int player_is_black,
                             int max_depth, long time_limit_ms, int endgame_threshold);

/*
 * 終盤完全読み: 評価関数を使わず、ゲーム終了までの石差を正確に読み切って最善手を返す。
 * empty_count が大きすぎる(概ね14〜16マス超)と非常に時間がかかるので注意。
 */
SearchResult endgame_search(Bitboard black, Bitboard white, int player_is_black, long time_limit_ms);

/*
 * 難易度(レベル)付きの着手選択。level は 1〜5（範囲外は丸める）。
 *   5: 最強。find_best_move(深さ上限30, time_limit_ms, 終盤16マス完全読み)と同じ
 *   4: 3手読み・終盤6マス完全読み。たまに(10%)ランダムな手を打つ
 *   3: 2手読み（終盤読み切りなし）。ときどき(25%)ランダムな手を打つ
 *   2: 1手先の評価だけで判断。半分の確率でランダムな手を打つ
 *   1: 接待用。1手先の評価で「一番悪い手」を優先して選ぶ（角も平気で譲る）
 * レベル1〜4は rand() を使うので、呼び出し側で srand() しておくこと。
 */
#define LEVEL_MIN 1
#define LEVEL_MAX 5
SearchResult find_move_by_level(Bitboard black, Bitboard white, int player_is_black,
                                 int level, long time_limit_ms);

#endif
