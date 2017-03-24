//
// Author  : Sai
// Created : Oct 2012
//
// extra_ticket_master_fmt.h -
//

#ifndef _omronsoft_pub_extra_ticket_master_fmt_h_
#define _omronsoft_pub_extra_ticket_master_fmt_h_

#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <omronsoft/cbm.h>
#include <omronsoft/sa1/pub/tablereader_extra_tick.h>
#include <omronsoft/sa1/pub/settingdata.h>

using namespace std;
using namespace omronsoft::pub::settingdata;

namespace omronsoft {
namespace pub {
namespace extraticketmaster {

static const int MASTER_NUM     = 2;     //マスタ数
static const int ISSUE_NUM      = 4;     //枚数
static const int TICKET_NUM     = 100;   //券種数
static const int ACCOUNT_NUM    = 50;    //口座数
static const int BITCOUNT       = 1;     //ビットマップ黒と白
static const int Y_MAX          = 480;   //券面ファイルに横最大サイズ

static const char* const SET_STATION = "SET_STATION";                   // 設置駅コード
static const char* const AREA_CODE   = "AREA_CODE";                     //   地域コード
static const char* const USER_CODE   = "USER_CODE";                     //   ユーザーコード
static const char* const CODE        = "CODE";                          //   設置駅線区駅順
static const char* const HOST_CODE   = "HOST_CODE";                     //   ホスト用設置駅コード

static const char* const EXTRA_TICKET_FACE_NAME = "extra_ticket";
static const char* const UNDER_BAR = "_";
static const char* const TICKET_FACE_EXT = ".tf";
static const char* const BITMAP_EXT = ".bmp";
static const char* const EXTRA_TICKET_FACE_PATH = "..\\ticketface\\tickettable\\";
static const char* const EXTRA_TICKET_BMP_PATH = "..\\ticketface\\bitmaps\\extra_ticket\\";

enum EncodeType {
	ENCODE_EDO          = 1,              // エド券
	ENCODE_PASS         = 2,              // 定期券               ←使用中
	ENCODE_FIX          = 3,              // 発着固定             ←使用中
	ENCODE_FIX_SECTION  = 4,              // 発着固定＋区数式
	ENCODE_SECTION      = 5,              // 区数式
    ENCODE_EDO_NOENCODE = 6,              // エド型白券
    ENCODE_BIG_NOENCODE = 7,              // 大型白券             ←使用中
};

enum ErrorDetail {
  ERROR_OK,
  ERROR_STATION,
  ERROR_OTHER,
};

//線区駅順用
struct LineCode {
	unsigned char line;
	unsigned char station;
	LineCode() : line(0), station(0) {}
};


// ヘッダ 
//class Prefix {
//public:
//	unsigned char set_version_;     //設定バージョン（マスタバージョン通知の設定バージョン）
//	unsigned char master_version_;  //マスタバージョン（マスタデータ要求のバージョン）
//	unsigned long size_;            //データサイズ
//	unsigned long sum_;             //サム値
//
//	std::string st_set_version_;       //設定バージョン（文字列）
//	std::string st_master_version_;    //マスタバージョン（文字列）
//
//	Prefix();
//	virtual ~Prefix();
//
//	bool internalize(const char*& data);
//};


// 企画券共通ヘッダ（１世代分）
class EachComData {
public:

	Timestamp valid_date_;                                // 適用日
	unsigned short data_version_;                         // 企画券データバージョン
	Timestamp data_create_time_;                          // 企画券データ作成日付・時分
	unsigned short master_version_;                       // 企画券マスタバージョン
	Timestamp master_create_time_;                        // 企画券マスタ作成日付・時分
	unsigned short format_version_;                       // 企画券フォーマットバージョン
	Timestamp format_create_time_;                        // 企画券フォーマット作成日付・時分
	unsigned short master_record_count_;                  // 企画券マスタレコード件数
//	unsigned short right_display_;                        // 企画券表示設定（右）
//	unsigned short left_display_;                         // 企画券表示設定（左）
//	unsigned short right_display_sp_;                     // 企画券表示設定（右）共同使用駅用
//	unsigned short left_display_sp_;                      // 企画券表示設定（左）共同使用駅用
	unsigned short button_display_order_[ACCOUNT_NUM];    // 口座ボタン表示順序
	unsigned short button_display_order_sp_[ACCOUNT_NUM]; // 口座ボタン表示順序  共同使用駅用
	unsigned short account_number_;                       // 口座数
//	unsigned short account_number_sp_;                    // 口座数              共同使用駅用
	std::string st_data_version_;                         // 企画券データバージョン（文字列）

	EachComData();
	virtual ~EachComData();

	bool internalize(const char*& data);
};


// 企画券共通ヘッダ
class ComPrefix {
public:

	unsigned long station_setting_code_;       // 設置駅コード
	Timestamp data_delivery_time_;             // 企画券データ(配信データ) 作成日付・時分
	unsigned long newdata_offset_;             // 新データオフセット
	unsigned long curdata_offset_;             // 現行データオフセット
	EachComData comdata_[MASTER_NUM];          // 共通データ１、２

	ComPrefix();
	virtual ~ComPrefix();

	bool internalize(const char*& data);
};

// 企画券券種情報
class TicketData {
public:

	unsigned short extra_ticket_no_; // 企画券No
	unsigned short issue_data_kind_no_; // 発売データ種別No
	unsigned short issue_data_no_; // 発売データNo
	unsigned short refund_data_kind_no_; // 払戻データ種別No
	unsigned short refund_data_no_; // 払戻データNo
//	unsigned long item_code_; // 商品コード
//	unsigned short start_station_code_; // 発駅コード
//	unsigned short start_company_code_; // 発社コード
//	unsigned short junction_station_code1_; // 接続駅１コード
//	unsigned short junction_company_code1_; // 接続社１コード
//	unsigned short junction_station_code2_; // 接続駅２コード
//	unsigned short junction_company_code2_; // 接続社２コード
//	unsigned short junction_station_code3_; // 接続駅３コード
//	unsigned short junction_company_code3_; // 接続社３コード
//	unsigned short arrive_station_code_; // 着駅コード
//	unsigned short arrive_company_code_; // 着社コード
	unsigned short issue_amount_adult_; // 発売金額（大人）
	unsigned short issue_amount_child_; // 発売金額（小児）(名臨高：0xFFFFセットのとき小児のみ発売禁止とする)
	unsigned short issue_amount_discount_adult_; // 発売金額（大人・障害割引）
	unsigned short issue_amount_discount_child_; // 発売金額（小児・障害割引）
	unsigned short division_amount_adult_; // 区数金額（大人）
	unsigned short division_amount_child_; // 区数金額（小児）
	unsigned short other_company_amount_adult_; // 他社取分金額（大人）
	unsigned short other_company_amount_child_; // 他社取分金額（小児）
	unsigned short other_company_amount_dis_adult_; // 他社取分金額（大人）
	unsigned short other_company_amount_dis_child_; // 他社取分金額（小児）
	unsigned short ticketface1_fare_adult_; // １枚目表示運賃（大人）
	unsigned short ticketface1_fare_child_; // １枚目表示運賃（小児）
	unsigned short ticketface2_fare_adult_; // ２枚目表示運賃（大人）
	unsigned short ticketface2_fare_child_; // ２枚目表示運賃（小児）
	unsigned short ticketface3_fare_adult_; // ３枚目表示運賃（大人）
	unsigned short ticketface3_fare_child_; // ３枚目表示運賃（小児）
	unsigned short ticketface4_fare_adult_; // ４枚目表示運賃（大人）
	unsigned short ticketface4_fare_child_; // ４枚目表示運賃（小児）
	unsigned short issue_num_; // 発行枚数
	Timestamp issue_start_date_; // 発売開始年月日
	Timestamp issue_end_date_; // 発売終了年月日
	bool is_indefinite_; // 無期限であるかどうか 発売終了年月日「99999999」無期限とする
	Timestamp pre_issue_start_date_; // 前売開始年月日
	bool is_pre_issue_; // 前売があるかどうか 前売開始年月日「99999999」前売なしとする
//	unsigned short v_valid_date_count_; // 多機能券売機用有効日数
	unsigned short cm_valid_date_count_; // 窓処用有効日数
	Timestamp event_start_date_; // イベント固定有効開始日
	Timestamp event_end_date_; // イベント固定有効終了日
//	bool v_issue_permit_; // 多機能券売機発売可否フラグ　true 可, false 否
//	bool v_refund_permit_; // 多機能券売機払戻可否フラグ　true 可, false 否
	bool cm_issue_permit_; // 窓処発売可否フラグ　true 可, false 否
	bool cm_refund_permit_; // 窓処払戻可否フラグ　true 可, false 否
	bool is_holiday_; // 土休日フラグ　true 土休日, false 通常日
	unsigned short limit_weekday_set_start_hour_; // 企画券制限設定（平日）開始時
	unsigned short limit_weekday_set_start_minute_; // 企画券制限設定（平日）開始分
	unsigned short limit_weekday_set_end_hour_; // 企画券制限設定（平日）終了時
	unsigned short limit_weekday_set_end_minute_; // 企画券制限設定（平日）終了分
	unsigned short limit_holiday_set_start_hour_; // 企画券制限設定（休日）開始時
	unsigned short limit_holiday_set_start_minute_; // 企画券制限設定（休日）開始分
	unsigned short limit_holiday_set_end_hour_; // 企画券制限設定（休日）終了時
	unsigned short limit_holiday_set_end_minute_; // 企画券制限設定（休日）終了分
	bool is_fare_change_; // 運改マーク　true 可, false 否
	unsigned short button_index_; // 口座ボタン表示順序

	TicketData();
	virtual ~TicketData();

	bool internalize(const char*& data);
};


// 企画券マスタ
class ExtraTicketMasterData {
public:

	TicketData ticket_data_[TICKET_NUM];        // １００券種

	// 世代（１：新、２：現行）
	int generation_;
	// マスタ数
    int ticketnum_;

	ExtraTicketMasterData();
	virtual ~ExtraTicketMasterData();

  bool internalize(const char*& data, int generation, int ticketnum);
};


// 企画券グループテーブル
class ExtraTicketGroupTable {
public:

	unsigned short group_table_record_num_; // グループテーブル登録レコード数

	struct GroupData {       // グループデータ
		unsigned short no;     //   企画券No
		std::string name;      //   企画券名称
//		unsigned short attribute; //  属性
		unsigned short ticket1_no; // 一枚目フォーマットNo
		unsigned short ticket2_no; // 二枚目フォーマットNo
		unsigned short ticket3_no; // 三枚目フォーマットNo
		unsigned short ticket4_no; // 四枚目フォーマットNo
		std::string v_button_word_jp_1;
		std::string v_button_word_jp_2;
		std::string v_button_word_en_1;
		std::string v_button_word_en_2;
		std::string cm_button_word_jp_1;
		std::string cm_button_word_jp_2;
//		GroupData() : no(0), name(""), attribute(0), ticket1_no(0), ticket2_no(0), ticket3_no(0),ticket4_no(0),
		GroupData() : no(0), name(""), ticket1_no(0), ticket2_no(0), ticket3_no(0),ticket4_no(0),
			v_button_word_jp_1(""), v_button_word_jp_2(""), v_button_word_en_1(""), v_button_word_en_2(""),
			cm_button_word_jp_1(""), cm_button_word_jp_2("") {}
	};

	vector<GroupData> group_data_;

	ExtraTicketGroupTable();
	virtual ~ExtraTicketGroupTable();

	bool internalize(const char*& data);
};


// 企画券フォーマットテーブル
class ExtraTicketFormatTable {
public:

	unsigned short format_table_record_num_; // フォーマットテーブル登録レコード数

	struct FMBigstEncode {       // FM大型券
		unsigned short ticket_kind3;      // 券種３　１５固定
		LineCode start1;  // 発駅１
		LineCode start2;  // 発駅２
		LineCode start3;  // 発駅３
		LineCode arrive1; // 着駅１
		LineCode arrive2; // 着駅２
		LineCode arrive3; // 着駅３
		unsigned short via_table_no;      // 経由テーブルNo
		LineCode via1;                     //経由1線区駅順
		LineCode via2;                     //経由2線区駅順
		LineCode via3;                     //経由3線区駅順
		LineCode via4;                     //経由4線区駅順
		LineCode via5;                     //経由5線区駅順
		LineCode via6;                     //経由6線区駅順
		LineCode via7;                     //経由7線区駅順
		LineCode via8;                     //経由8線区駅順
		LineCode via9;                     //経由9線区駅順
		LineCode via10;                    //経由10線区駅順
		LineCode free_area_code;           //フリー区間コード
		unsigned short ticket_kind4_detail; // 券種４詳細
		unsigned char roundtrip_info;           //往復情報 0:指定なし、1:往券、2:復券
		unsigned char ticket_type; // 0:指定なし両方ビットオフ、1:時差、2:土休、以外:両方ビットオフ
		unsigned long item_code; // 商品コード
		unsigned char discount_code;       // 割引コード　０～１５
		FMBigstEncode() : ticket_kind3(0), via_table_no(0), ticket_kind4_detail(0),
			roundtrip_info(0), ticket_type(0), item_code(0), discount_code(0) {}
	};

	struct FMPassEncode {       // FM定期券
		unsigned short ticket_kind3;      // 券種３　９固定
		LineCode start;  // 発駅
		LineCode arrive; // 着駅
		unsigned short via_table_no;      // 経由テーブルNo
		LineCode via1;                     //経由1線区駅順
		LineCode via2;                     //経由2線区駅順
		LineCode via3;                     //経由3線区駅順
		LineCode via4;                     //経由4線区駅順
		LineCode via5;                     //経由5線区駅順
		LineCode via6;                     //経由6線区駅順
		LineCode via7;                     //経由7線区駅順
		LineCode via8;                     //経由8線区駅順
		bool doubleness_bit;                // 二重化　true:あり(８トラック)、false:なし(４トラック)
		FMPassEncode() : ticket_kind3(0), via_table_no(0), doubleness_bit(false) {}
	};

	struct FormatData {       // グループデータ
		unsigned short no;     //   フォーマットNo
		std::string name;      //   企画券名称
		unsigned short ticket_kind; //  券種
//		unsigned short attribute; //  属性
		bool is_amount_print;     // 金額印字有無　true 有, false 無
		LineCode start_station; // 発駅
		bool is_numbering_print;  // ナンバリング印字有無　true 有, false 無
		FMBigstEncode fmbigst_encode;
		FMPassEncode fmpass_encode;
//		FormatData() : no(0), name(""), ticket_kind(0), attribute(0), is_amount_print(false), is_numbering_print(false) {}
		FormatData() : no(0), name(""), ticket_kind(0), is_amount_print(false), is_numbering_print(false) {}
	};

	vector<FormatData> format_data_;

	ExtraTicketFormatTable();
	virtual ~ExtraTicketFormatTable();

	bool internalize(const char*& data);
};


//// 企画券経由パターンテーブル経由８定期券用
//class ExtraTicketVia8Table {
//public:
//
//	unsigned short via8_table_record_num_; // 経由８テーブル登録レコード数
//
//	struct Via8Info {       // 経由情報
//		unsigned short line;      // 線区
//		unsigned short station_lower;      // 駅順下限
//		unsigned short station_upper;      // 駅順上限
//		unsigned short via_num;      // 経由駅数
//		LineCode via1;                     //経由1線区駅順
//		LineCode via2;                     //経由2線区駅順
//		LineCode via3;                     //経由3線区駅順
//		LineCode via4;                     //経由4線区駅順
//		LineCode via5;                     //経由5線区駅順
//		LineCode via6;                     //経由6線区駅順
//		LineCode via7;                     //経由7線区駅順
//		LineCode via8;                     //経由8線区駅順
//
//		Via8Info() : line(0), station_lower(0), station_upper(0), via_num(0) {}
//	};
//
//	struct Via8Data {       // 経由８データ
//		unsigned short via_table_no;      // 経由テーブルNo
//		unsigned short via_info_num;      // 経由情報数
//		vector<Via8Info> via8_info;       // 経由情報
//
//		Via8Data() : via_table_no(0), via_info_num(0) {}
//	};
//
//	vector<Via8Data> via8_data_;
//
//	ExtraTicketVia8Table();
//	virtual ~ExtraTicketVia8Table();
//
//	bool internalize(const char*& data);
//};
//
//
//// 企画券経由パターンテーブル経由１０大型券用
//class ExtraTicketVia10Table {
//public:
//
//	unsigned short via10_table_record_num_; // 経由１０テーブル登録レコード数
//
//	struct Via10Info {       // 経由情報
//		unsigned short line;      // 線区
//		unsigned short station_lower;      // 駅順下限
//		unsigned short station_upper;      // 駅順上限
//		unsigned short via_num;      // 経由駅数
//		LineCode via1;                     //経由1線区駅順
//		LineCode via2;                     //経由2線区駅順
//		LineCode via3;                     //経由3線区駅順
//		LineCode via4;                     //経由4線区駅順
//		LineCode via5;                     //経由5線区駅順
//		LineCode via6;                     //経由6線区駅順
//		LineCode via7;                     //経由7線区駅順
//		LineCode via8;                     //経由8線区駅順
//		LineCode via9;                     //経由9線区駅順
//		LineCode via10;                     //経由10線区駅順
//
//		Via10Info() : line(0), station_lower(0), station_upper(0), via_num(0) {}
//	};
//
//	struct Via10Data {       // 経由１０データ
//		unsigned short via_table_no;      // 経由テーブルNo
//		unsigned short via_info_num;      // 経由情報数
//		vector<Via10Info> via10_info;       // 経由情報
//
//		Via10Data() : via_table_no(0), via_info_num(0) {}
//	};
//
//	vector<Via10Data> via10_data_;
//
//	ExtraTicketVia10Table();
//	virtual ~ExtraTicketVia10Table();
//
//	bool internalize(const char*& data);
//};


// 企画券券面フォーマットデータ
class ExtraTicketFaceData {
public:

	unsigned short ticketface_data_record_num_; // 券面フォーマットデータ登録レコード数

	struct Illustration {       // イラストデータ
//		bool is_v_print;  // 券売機印字有無　true 有, false 無
		bool is_cm_print;  // 窓処印字有無　true 有, false 無
		unsigned short point_x;     //   X座標
		unsigned short point_y;     //   Y座標
		unsigned short width;     //   横サイズ
		unsigned short height;     //   縦サイズ
		unsigned long illust_offset;     //   イラストデータの先頭アドレス
//		Illustration() : is_v_print(false), is_cm_print(false), point_x(0), point_y(0), 
		Illustration() : is_cm_print(false), point_x(0), point_y(0), 
			width(0), height(0), illust_offset(0) {}
	};

	struct PrintInfo {          // 注意文
		unsigned short word_line_num;     //   １行のバイト数
		std::string print_word;       // 注意文
		PrintInfo() : word_line_num(0), print_word("") {}
	};

	struct TicketFaceData {       // 券面フォーマットデータ
		unsigned short no;     //   フォーマットNo
		std::string format_code; // ベース券面コード（Ａ～Ｒ）全角
		bool is_numbering_print;  // ナンバリング印字有無　true 有, false 無
		unsigned short illustration_num;     //   イラスト設定数
		vector<Illustration> illustration_info;       // イラスト情報
		unsigned short print_info_num;     //   注意文設定数
		vector<PrintInfo> print_info;       // 注意文
		TicketFaceData() : no(0), format_code(""), is_numbering_print(false), illustration_num(0),
			print_info_num(0) {}
	};

	vector<TicketFaceData> ticketface_data_;

	ExtraTicketFaceData();
	virtual ~ExtraTicketFaceData();

	bool internalize(const char*& data);
};


// 企画券フォーマットデータ情報
class TicketFormatData {
public:

	unsigned short extra_ticket_no_;  // 企画券No
    std::string extra_ticket_name_;   // 企画券名称
    unsigned short encode_num_;       // 企画券エンコード枚数
	std::string v_button_word_jp_1_;  // 口座ボタン券売機和文1段目
	std::string v_button_word_jp_2_;  // 口座ボタン券売機和文2段目
	std::string v_button_word_en_1_;  // 口座ボタン券売機英文1段目
	std::string v_button_word_en_2_;  // 口座ボタン券売機英文2段目
	std::string cm_button_word_jp_1_; // 口座ボタン窓処和文1段目
	std::string cm_button_word_jp_2_; // 口座ボタン窓処和文2段目

	struct IssueInfo {       // 発券情報
		unsigned short ticket_kind;
		std::string ticketface_name;
		bool is_amount_print;
		LineCode start_station;
		bool is_numbering_print;
		unsigned short fmbigst_ticket_kind3;
		LineCode fmbigst_start_station1;
		LineCode fmbigst_start_station2;
		LineCode fmbigst_start_station3;
		LineCode fmbigst_arrive_station1;
		LineCode fmbigst_arrive_station2;
		LineCode fmbigst_arrive_station3;
		LineCode fmbigst_via_station1;
		LineCode fmbigst_via_station2;
		LineCode fmbigst_via_station3;
		LineCode fmbigst_via_station4;
		LineCode fmbigst_via_station5;
		LineCode fmbigst_via_station6;
		LineCode fmbigst_via_station7;
		LineCode fmbigst_via_station8;
		LineCode fmbigst_via_station9;
		LineCode fmbigst_via_station10;
		LineCode fmbigst_free_area_code;
		unsigned short fmbigst_ticket_kind4_detail;
		unsigned char fmbigst_roundtrip_info;
		unsigned char fmbigst_ticket_type;
		unsigned long fmbigst_item_code;
		unsigned char fmbigst_discount_code;
		unsigned short fmpass_ticket_kind3;
		LineCode fmpass_start_station;
		LineCode fmpass_arrive_station;
		LineCode fmpass_via_station1;
		LineCode fmpass_via_station2;
		LineCode fmpass_via_station3;
		LineCode fmpass_via_station4;
		LineCode fmpass_via_station5;
		LineCode fmpass_via_station6;
		LineCode fmpass_via_station7;
		LineCode fmpass_via_station8;
		bool fmpass_doubleness_bit;
		std::string format_code;
		IssueInfo() : ticket_kind(0), ticketface_name(""), is_amount_print(false), is_numbering_print(false), 
			fmbigst_ticket_kind3(0), fmbigst_ticket_kind4_detail(0), fmbigst_roundtrip_info(0), 
			fmbigst_ticket_type(0), fmbigst_item_code(0), fmbigst_discount_code(0),
            fmpass_ticket_kind3(0), fmpass_doubleness_bit(false),format_code("") {}
	};

	IssueInfo issue_info_[ISSUE_NUM];       //発券情報

	TicketFormatData();
	virtual ~TicketFormatData();

};


// 企画券フォーマット
class ExtraTicketFormatData {
public:

	unsigned short station_code_;   // 駅コード
	Timestamp operate_date_;        // 運用日付
	unsigned short record_num_;     // レコード数

	// データヘッダ
	char identify_word_[20]; // 識別文字列
	Timestamp effective_start_date_; // 有効開始日付
	unsigned short tool_version_; // ツールバージョン
	unsigned short format_version_; // 企画券フォーマットデータバージョン
	Timestamp format_create_time_; // 企画券フォーマットデータ作成時間
//	unsigned long author_;

	// インデックステーブル
	unsigned long index_group_table_; // index グループテーブル
	unsigned long index_format_table_; // index フォーマットテーブル
//	unsigned long index_via8_table_; // index 経由８パターンテーブル
//	unsigned long index_via10_table_; // index 経由１０パターンテーブル
	unsigned long index_ticketface_data_; // index 券面フォーマットデータ

	// グループテーブル
	ExtraTicketGroupTable group_table_;

	// フォーマットテーブル
	ExtraTicketFormatTable format_table_;

//	// 企画券経由パターンテーブル経由８定期券用
//	ExtraTicketVia8Table via8_table_;

//	// 企画券経由パターンテーブル経由１０大型券用
//	ExtraTicketVia10Table via10_table_;

	// 企画券券面フォーマットデータ
	ExtraTicketFaceData ticketface_format_data_;

	// 企画券フォーマットデータ情報
	vector<TicketFormatData> ticketformat_data_;

	// 世代（１：新、２：現行）
	int generation_;

	// 自駅の線区駅順
	LineCode my_station_code_;

	ExtraTicketFormatData();
	virtual ~ExtraTicketFormatData();

	bool internalize(const char*& data,  int generation);
	void station_code_change(LineCode station_code_in, LineCode& station_code_out);
    bool ticketface_data_bmp_create(const char* data);
    bool ticketface_data_file_create(const char* data);
    bool ticketformat_data_set();
};


// 企画券データ（１世代分）
class EachExtraTicketData {
public:
	ExtraTicketMasterData master_;
	ExtraTicketFormatData format_;

	// 世代（１：新、２：現行）
	int generation_;
	// マスタ数
    int ticketnum_;

	EachExtraTicketData();
	virtual ~EachExtraTicketData();

	bool internalize(const char*& data, int generation, int ticketnum);
};


// 企画券データ
class ExtraTicketData {
public:
//	Prefix         prefix_;                   // 通信部ヘッダ 名臨高がない
	ComPrefix      comprefix_;                // 共通ヘッダ
	EachExtraTicketData eachdata_[MASTER_NUM];  // データ１、２

	ExtraTicketData();
	virtual ~ExtraTicketData();

	bool internalize(const char* data, long size, int& error_detail);
};

class ETFmtBmpMaker {
public:
  ETFmtBmpMaker();
  virtual ~ETFmtBmpMaker();
  bool MakeFontBmp(const char* filename, bool isYOKO, unsigned short width, unsigned short height, const unsigned char* data);
  unsigned long fileheader(FILE* fp, unsigned long size, unsigned long offset);
  unsigned long bitmapheader(FILE * fp, unsigned long width, unsigned long height, unsigned long size);
  unsigned long rgbquad(FILE * fp);
  unsigned long imagedata(FILE * fp, unsigned long datalng, const unsigned char* data, unsigned long offset);
  void font2bit(unsigned short width, unsigned short height,
                const unsigned char* pFont, unsigned char*& pOut, unsigned long LineByte, bool isYOKO);
  void setdot_yoko(unsigned long x, unsigned long y, unsigned char*& pOut, unsigned long LineByte);
  void setdot_tate_h(unsigned long x, unsigned long y, unsigned char*& pOut, unsigned long LineByte, unsigned long height);
  void setdot_tate_w(unsigned long x, unsigned long y, unsigned char*& pOut, unsigned long LineByte, unsigned long width);
};

class TicketFaceFileEdit {
public:

  TicketFaceFileEdit(std::string filename);
  virtual ~TicketFaceFileEdit();
  bool open(std::string basefilename);
  bool replaceFixedBmpField(int x, int y, std::string stReplaceTo);
  bool removeNumFlexibleField(int x, int y, std::string stKeyword);
  bool removePatFlexibleField(int x, int y);
  bool replaceFixedTxtField(vector<std::string> stReplaceToAry);
  bool save();

private:
  vector<std::string> txt_buff_;
  std::string file_name_;
};

class ExtraTicketUtils {
public:
  ExtraTicketUtils();
  virtual ~ExtraTicketUtils();
  std::string string_trim(const std::string& str, const char* delim=" \t\r\n");
};

#include "extra_ticket_master_fmt.i"

}
}
}

#endif // _omronsoft_pub_extra_ticket_master_fmt_h_
// end of file
