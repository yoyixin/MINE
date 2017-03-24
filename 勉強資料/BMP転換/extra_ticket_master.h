//
// Author  : Sai
// Created : Oct 2012
//
// extra_ticket_master.h

#ifndef _omronsoft_pub_extra_ticket_master_h_
#define _omronsoft_pub_extra_ticket_master_h_

#include <omronsoft/multiprops.h>
#include <omronsoft/datetime.h>
#include <omronsoft/sa1.h>
#include "../master.h"

namespace omronsoft {
namespace pub {
namespace extraticketmaster {

class ExtraTicketAccountRep;
class SA1_STANDARD_LIB_API ExtraTicketAccount {
public:

	enum DAYS {
		WEEKDAY   = 0,  //平日
		HOLIDAY   = 1,  //土休日
	};

	enum PLANNINGKIND {
		PLANNING_UNKNOWN = 0,
		PLANNING_PASS = 2,        // 定期券
		PLANNING_BIG_ST = 3,      // 大型券
		PLANNING_NO_ENCODE = 7,   // ノーエンコード券
	};

	//企画券No
	unsigned short extra_ticket_no() const;
	//発売データ種別No
	unsigned short issue_data_kind_no() const;
	//発売データNo
	unsigned short issue_data_no() const;
	//払戻データ種別No
	unsigned short refund_data_kind_no() const;
	//払戻データNo
	unsigned short refund_data_no() const;
	//発売金額（大人）
	unsigned short issue_amount_adult() const;
	//発売金額（小児）
	unsigned short issue_amount_child() const;
	//n枚目表示運賃（大人）
	unsigned short ticket_fare_adult(unsigned short count) const;
	//n枚目表示運賃（小児）
	unsigned short ticket_fare_child(unsigned short count) const;
	//発行枚数
	unsigned short issue_num() const;
	//発売開始年月日
	Timestamp issue_start_date() const;
	//発売終了年月日
	Timestamp issue_end_date() const;
	//無期限であるかどうか
	bool is_indefinite() const;
	//前売開始年月日
	Timestamp pre_issue_start_date() const;
	//前売があるかどうか
	bool is_pre_issue() const;
//	//多機能券売機用有効日数
//	unsigned short v_valid_date_count() const;
	//窓処用有効日数有効日数
	unsigned short cm_valid_date_count() const;
	//イベント固定有効開始日
	Timestamp event_start_date() const;
	//イベント固定有効終了日
	Timestamp event_end_date() const;
//	//多機能券売機発売可否
//	bool v_issue_permit() const;
//	//多機能券売機払戻可否
//	bool v_refund_permit() const;
	//窓口処理機発売可否
	bool cm_issue_permit() const;
	//窓口処理機払戻可否
	bool cm_refund_permit() const;
	//土休日
	bool is_holiday() const;
	//企画券制限設定（平日）開始時
	unsigned short limit_weekday_set_start_hour() const;
	//企画券制限設定（平日）開始分
	unsigned short limit_weekday_set_start_minute() const;
	//企画券制限設定（平日）終了時
	unsigned short limit_weekday_set_end_hour() const;
	//企画券制限設定（平日）終了分
	unsigned short limit_weekday_set_end_minute() const;
	//企画券制限設定（休日）開始時
	unsigned short limit_holiday_set_start_hour() const;
	//企画券制限設定（休日）開始分
	unsigned short limit_holiday_set_start_minute() const;
	//企画券制限設定（休日）終了時
	unsigned short limit_holiday_set_end_hour() const;
	//企画券制限設定（休日）終了分
	unsigned short limit_holiday_set_end_minute() const;
//    //券売機発売可能チェック true:発売可、false:発売不可、preはtrue場合は事前確認　時刻制限判断せず
//    bool sale_judge_v(Timestamp date, int days, bool pre = false) const;
    //窓処発売可能チェック true:発売可、false:発売不可、preはtrue場合は事前確認　時刻制限判断せず
    bool sale_judge_cm(Timestamp date, int days, bool pre = false) const;
	//企画券名称
    const char* extra_ticket_name() const;
	//口座ボタン券売機和文1段目
	const char* v_button_word_jp_1() const;
	//口座ボタン券売機和文2段目
	const char* v_button_word_jp_2() const;
	//口座ボタン券売機英文1段目
	const char* v_button_word_en_1() const;
	//口座ボタン券売機英文2段目
	const char* v_button_word_en_2() const;
	//口座ボタン窓処和文1段目
	const char* cm_button_word_jp_1() const;
	//口座ボタン窓処和文2段目
	const char* cm_button_word_jp_2() const;
	//券種（n枚目）
	unsigned short ticket_kind(unsigned short count) const;
	//券面ファイル名前（n枚目）
	const char* ticketface_name(unsigned short count) const;
	//金額印字有無（n枚目）
	bool is_amount_print(unsigned short count) const;
	//発駅（n枚目）
	void start_station(unsigned short count, unsigned char& line, unsigned char& station) const;
	//ナンバリング印字有無（n枚目）
	bool is_numbering_print(unsigned short count) const;
	//口座ボタン表示順序
	unsigned short button_index() const;
	//ベース券面コード（Ａ～Ｒ）で判断する割引有無
	bool discount_by_format_code(unsigned short count) const;

	//------------大型券エンコード用------------
	//券種３（大型券）（n枚目）
	unsigned short fmbigst_ticket_kind3(unsigned short count) const;
	//発駅１（大型券）（n枚目）
	void fmbigst_start_station1(unsigned short count, unsigned char& line, unsigned char& station) const;
	//発駅２（大型券）（n枚目）
	void fmbigst_start_station2(unsigned short count, unsigned char& line, unsigned char& station) const;
	//発駅３（大型券）（n枚目）
	void fmbigst_start_station3(unsigned short count, unsigned char& line, unsigned char& station) const;
	//着駅１（大型券）（n枚目）
	void fmbigst_arrive_station1(unsigned short count, unsigned char& line, unsigned char& station) const;
	//着駅２（大型券）（n枚目）
	void fmbigst_arrive_station2(unsigned short count, unsigned char& line, unsigned char& station) const;
	//着駅３（大型券）（n枚目）
	void fmbigst_arrive_station3(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由１（大型券）（n枚目）
	void fmbigst_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由２（大型券）（n枚目）
	void fmbigst_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由３（大型券）（n枚目）
	void fmbigst_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由４（大型券）（n枚目）
	void fmbigst_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由５（大型券）（n枚目）
	void fmbigst_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由６（大型券）（n枚目）
	void fmbigst_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由７（大型券）（n枚目）
	void fmbigst_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由８（大型券）（n枚目）
	void fmbigst_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由９（大型券）（n枚目）
	void fmbigst_via_station9(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由１０（大型券）（n枚目）
	void fmbigst_via_station10(unsigned short count, unsigned char& line, unsigned char& station) const;
	//フリー区間コード（大型券）（n枚目）
	void  fmbigst_free_area_code(unsigned short count, unsigned char& line, unsigned char& station) const;
	//券種４詳細（大型券）（n枚目）
	unsigned short fmbigst_ticket_kind4_detail(unsigned short count) const;
	//往復情報（大型券）（n枚目）
	unsigned char fmbigst_roundtrip_info(unsigned short count) const;
	//時差・土休（大型券）（n枚目）
	unsigned char fmbigst_ticket_type(unsigned short count) const;
	//商品コード（大型券）（n枚目）
	unsigned long fmbigst_item_code(unsigned short count) const;
	//割引コード（大型券）（n枚目）
	unsigned char fmbigst_discount_code(unsigned short count) const;


	//------------定期券エンコード用------------
	//券種３（定期券）（n枚目）
	unsigned short fmpass_ticket_kind3(unsigned short count) const;
	//発駅（定期券）（n枚目）
	void fmpass_start_station(unsigned short count, unsigned char& line, unsigned char& station) const;
	//着駅（定期券）（n枚目）
	void fmpass_arrive_station(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由１（定期券）（n枚目）
	void fmpass_via_station1(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由２（定期券）（n枚目）
	void fmpass_via_station2(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由３（定期券）（n枚目）
	void fmpass_via_station3(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由４（定期券）（n枚目）
	void fmpass_via_station4(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由５（定期券）（n枚目）
	void fmpass_via_station5(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由６（定期券）（n枚目）
	void fmpass_via_station6(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由７（定期券）（n枚目）
	void fmpass_via_station7(unsigned short count, unsigned char& line, unsigned char& station) const;
	//経由８（定期券）（n枚目）
	void fmpass_via_station8(unsigned short count, unsigned char& line, unsigned char& station) const;
	//２重化（定期券）（n枚目）
	bool fmpass_doubleness_bit(unsigned short count) const;

	ExtraTicketAccount();
	ExtraTicketAccount(const ExtraTicketAccount&);
	ExtraTicketAccount& operator=(const ExtraTicketAccount&);
	virtual ~ExtraTicketAccount();

private:
	friend class EachExtraTicketMasterRep;
	ExtraTicketAccountRep* rep_;
};


class EachExtraTicketMasterRep;
class SA1_STANDARD_LIB_API EachExtraTicketMaster {
public:

	// 他社線企画券口座検索時に、collaborationはtrueにする

	//券売機発売できる口座数取得
	unsigned short account_number_v(Timestamp date, int days, bool collaboration = false, bool pre = false) const;
	//窓処発売できる口座数取得
	unsigned short account_number_cm(Timestamp date, int days, bool collaboration = false, bool pre = false) const;
	//口座情報取得（次候補）
	bool next_one(ExtraTicketAccount& eta, bool collaboration = false);
	void first(bool collaboration = false);

	//口座情報取得（企画券No指定）
	bool get_account_ticket_no(unsigned short extra_ticket_no, ExtraTicketAccount& eta, bool collaboration = false) const;
	//口座情報取得（企画券No指定　窓処払戻用）
	bool get_account_ticket_no_refund_cm(unsigned short extra_ticket_no, ExtraTicketAccount& eta) const;

	//企画券表示設定（右）(券売機用)
	bool get_right_button(ExtraTicketAccount& eta, bool collaboration = false) const;
	//企画券表示設定（左）(券売機用)
	bool get_left_button(ExtraTicketAccount& eta, bool collaboration = false) const;

	EachExtraTicketMaster();
	EachExtraTicketMaster(const EachExtraTicketMaster&);
	EachExtraTicketMaster& operator=(const EachExtraTicketMaster&);
	virtual ~EachExtraTicketMaster();

private:
	friend class ExtraTicketMasterRep;
	EachExtraTicketMasterRep* rep_;
};


class ExtraTicketMasterRep;
class SA1_STANDARD_LIB_API ExtraTicketMaster : public omronsoft::pub::master::MasterData {
public:
	virtual ~ExtraTicketMaster();

	enum{NG,OK};  //data_set(),data_get()の戻り値

	enum MASTERID {
		NEW_VERSION = 1,
		CUR_VERSION = 2
	};

	enum ErrorDetail {
		ERROR_OK,
		ERROR_STATION,
		ERROR_OTHER,
	};

	struct PlanDataKind {
		unsigned short dataKindNo;
	    unsigned short dataNo;
	};

	typedef std::map<unsigned short, PlanDataKind> KindMap;
	typedef std::pair<unsigned short,PlanDataKind> KindElement;
	KindMap m_DataKindMap;

	int m_CurMasterID;

	//--------------------------------------------------------------------------
	//マスタデータ名取得 name()
	//戻り値：const char* マスタデータ名
	//--------------------------------------------------------------------------
	const char* name() const;

	//--------------------------------------------------------------------------
	//設定バージョン取得 version()
	//戻り値：const char* バージョン（データなし、または異常の場合はMasterDef::UNKNOWN）
	//--------------------------------------------------------------------------
	const char* version() const;

	//--------------------------------------------------------------------------
	//マスタバージョン取得 master_version()
	//戻り値：const char* バージョン（データなし、または異常の場合はMasterDef::UNKNOWN）
	//--------------------------------------------------------------------------
	const char* master_version() const;

	//--------------------------------------------------------------------------
	//マスタデータセット data_set()
	//引数： const char* data:マスタデータ
	//       long size:データサイズ(ヘッダ含む)
	//戻り値：int ０：異常、１：正常
	//--------------------------------------------------------------------------
	int data_set(const char* data, long size);

	//--------------------------------------------------------------------------
	//マスタデータ取得 data_get()
	//引数： const char*& data:マスタデータ
	//       long& size:データサイズ(ヘッダ含む)
	//戻り値：int ０：異常、１：正常
	//--------------------------------------------------------------------------
	int data_get(const char*& data, long& size) const;

	//--------------------------------------------------------------------------
	//有効開始年月日時分取得 valid_date()
	//引数： int master_id:マスタデータID マスタデータ中のmaster_id番目のデータの
	//                     有効開始年月日時分を取得   master_idは１以上
	//       Timestamp& valid_date:有効開始年、月、日、時、分が設定される
	//戻り値：int ０：取得失敗、１：取得成功
	//--------------------------------------------------------------------------
	int valid_date(int master_id, Timestamp& valid_date) const;

	//--------------------------------------------------------------------------
	//有効開始年月日時分 およびバージョン取得 valid_master()
	//引数： int master_id:マスタデータID マスタデータ中のmaster_id番目のデータの
	//                     有効開始年月日時分およびバージョンを取得   master_idは１以上
	//       Timestamp& valid_date:有効開始年、月、日、時、分が設定される
	//       const char*& master_version:有効企画券データバージョンが設定される
	//戻り値：int ０：取得失敗、１：取得成功
	//--------------------------------------------------------------------------
	int valid_master(int master_id, Timestamp& valid_date, const char*& master_version) const;

	//--------------------------------------------------------------------------
	//有効マスタ取得 valid_date()
	//引数： const Timestamp& real_date:実日付 有効マスタ判別用　年月日時分を指定する
	//       EachExtraTicketMaster& valid_master:有効マスタ（一世代分）
	//戻り値：int ０：取得失敗、１：取得成功
	//--------------------------------------------------------------------------
	int get_master(const Timestamp& real_date, EachExtraTicketMaster& valid_master) ;

	//--------------------------------------------------------------------------
	//券種リスト更新 update_kind_map()
	//引数：bool needjudge     master_idが判定必要の時に、trueにする
	//      bool collaboration 他社線企画券口座検索時に、trueにする
	//戻り値：int ０：更新失敗、１：更新成功
	//--------------------------------------------------------------------------
	bool update_kind_map(bool needjudge);

	//--------------------------------------------------------------------------
	//設置駅コード取得 station_setting_code()
	//戻り値：unsigned long 設置駅コード（データなし、または異常の場合は0）
	//--------------------------------------------------------------------------
	unsigned long station_setting_code() const;

	//--------------------------------------------------------------------------
	//企画券データ(配信データ)作成日取得 master_data_delivery_time()
	//戻り値：int
	//--------------------------------------------------------------------------
	int master_data_delivery_time(Timestamp& delivery_time) const;

	//デバッグ用
	//  void print();

private:
	ExtraTicketMaster();
	ExtraTicketMaster(const ExtraTicketMaster&);
	ExtraTicketMaster& operator=(const ExtraTicketMaster&);

	ExtraTicketMasterRep* rep_;
	static ExtraTicketMaster theInstance;

public:
	virtual const char* getMasterFileName() const;
	virtual unsigned long sum() const; 
};


}
}
}


#endif // _omronsoft_pub_extra_ticket_master_h_
// end of file
