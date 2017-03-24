//
// Author  : Sai
// Created : Oct 2012
//
// extra_ticket_master_fmt.i -
//

inline unsigned long to_long(unsigned char* c)
{
  char to_buf[4];
  char* cp = &to_buf[3];
  *cp-- = *c++;
  *cp-- = *c++;
  *cp-- = *c++;
  *cp   = *c;
  return *(unsigned long*)(to_buf);
}

inline unsigned short to_short(unsigned char* c)
{
  char to_buf[2];
  char* cp = &to_buf[1];
  *cp-- = *c++;
  *cp   = *c;
  return *(unsigned short*)(to_buf);
}

inline unsigned long bcd_to_bin8(unsigned long num){
  unsigned long ret = 0;
  ret += num / 0x10000000 * 10000000;
  num =  num % 0x10000000;
  ret += num / 0x1000000 * 1000000;
  num =  num % 0x1000000;
  ret += num / 0x100000 * 100000;
  num =  num % 0x100000;
  ret += num / 0x10000 * 10000;
  num =  num % 0x10000;
  ret += num / 0x1000 * 1000;
  num =  num % 0x1000;
  ret += num / 0x100 * 100;
  num =  num % 0x100;
  ret += num / 0x10 * 10;
  num =  num % 0x10;
  ret += num;
  return ret;
}

inline unsigned short bcd_to_bin4(unsigned short num){
  unsigned short ret = 0;
  ret += num / 0x1000 * 1000;
  num =  num % 0x1000;
  ret += num / 0x100 * 100;
  num =  num % 0x100;
  ret += num / 0x10 * 10;
  num =  num % 0x10;
  ret += num;
  return ret;
}

inline unsigned short bcd_to_bin2(unsigned char num){
  unsigned short ret = 0;
  ret += num / 0x10 * 10;
  num =  num % 0x10;
  ret += num;
  return ret;
}

// 通信部ヘッダ名臨高がないので、削除↓
////ヘッダ
//typedef struct _Prefix {
//  unsigned char ver1;
//  unsigned char ver2;
//  unsigned char sz[4];
//  unsigned char sm[4];
//} PREFIX;
//
//inline Prefix::Prefix()
//: set_version_(0),master_version_(0),size_(0),sum_(0)
//{}
//
//inline Prefix::~Prefix(){}
//
//inline bool Prefix::internalize(const char*& data)
//{
//  bool ret = false;
//  try {
//    char buf[sizeof PREFIX];
//    memset(buf, 0, sizeof buf);
//    memcpy(buf,data,sizeof buf);
//    PREFIX* orig = reinterpret_cast<PREFIX*>(buf);
//    set_version_    = orig->ver1;
//    master_version_ = orig->ver2;        
//    size_           = to_long(orig->sz);
//    sum_            = to_long(orig->sm);
//
//    std::ostringstream oss1, oss2;
//    oss1 << (int)orig->ver1;
//    st_set_version_ = oss1.str();
//
//    oss2 << (int)orig->ver2;
//    st_master_version_ = oss2.str();
//
//    data += sizeof PREFIX;
//    ret = true;
//  }
//  catch (...) {}
//  return ret;
//}
// 通信部ヘッダ名臨高がないので、削除↑


//企画券共通ヘッダ（１世代分）
typedef struct _ComData {
  unsigned char valid_date_year[2];
  unsigned char valid_date_month;
  unsigned char valid_date_day;
  unsigned char data_version[2];
  unsigned char data_create_time_year[2];
  unsigned char data_create_time_month;
  unsigned char data_create_time_day;
  unsigned char data_create_time_hour;
  unsigned char data_create_time_minute;
  unsigned char master_version[2];
  unsigned char master_create_time_year[2];
  unsigned char master_create_time_month;
  unsigned char master_create_time_day;
  unsigned char master_create_time_hour;
  unsigned char master_create_time_minute;
  unsigned char format_version[2];
  unsigned char format_create_time_year[2];
  unsigned char format_create_time_month;
  unsigned char format_create_time_day;
  unsigned char format_create_time_hour;
  unsigned char format_create_time_minute;
  unsigned char master_record_count[2];
//  unsigned char right_display[2];
//  unsigned char left_display[2];
//  unsigned char right_display_sp[2];
//  unsigned char left_display_sp[2];
  unsigned char reserved1[2];
  unsigned char reserved2[2];
  unsigned char reserved3[2];
  unsigned char reserved4[2];
  unsigned char button_display_order[100];
//  unsigned char button_display_order_sp[100];
  unsigned char reserved5[100];
  unsigned char reserved6[16];
} COMDATA;

inline EachComData::EachComData()
: valid_date_(0),data_version_(0),data_create_time_(0),master_version_(0),master_create_time_(0),
  format_version_(0),format_create_time_(0),master_record_count_(0),account_number_(0),
  st_data_version_("")
// ,right_display_(0),left_display_(0),
//  right_display_sp_(0),left_display_sp_(0),account_number_(0),account_number_sp_(0)
{
  for(int i = 0; i < ACCOUNT_NUM; ++i) {
    button_display_order_[i] = 0;
    button_display_order_sp_[i] = 0;
  }
}

inline EachComData::~EachComData(){}

inline bool EachComData::internalize(const char*& data)
{
  bool ret = false;
  try {
    char buf[sizeof COMDATA];
    memset(buf, 0, sizeof buf);
    memcpy(buf,data,sizeof buf);
    COMDATA* orig = reinterpret_cast<COMDATA*>(buf);
    valid_date_ = Timestamp(bcd_to_bin4(to_short(orig->valid_date_year)),
                                    bcd_to_bin2(orig->valid_date_month),
                                    bcd_to_bin2(orig->valid_date_day),
                                    0,
                                    0,
                                    0.0);

    data_version_ = to_short(orig->data_version);

    std::ostringstream oss1;
	oss1.fill('0');
	oss1 <<  std::setw(2) <<data_version_;
    st_data_version_ = oss1.str();

    data_create_time_ = Timestamp(bcd_to_bin4(to_short(orig->data_create_time_year)),
                                    bcd_to_bin2(orig->data_create_time_month),
                                    bcd_to_bin2(orig->data_create_time_day),
                                    bcd_to_bin2(orig->data_create_time_hour),
                                    bcd_to_bin2(orig->data_create_time_minute),
                                    0.0);

    master_version_ = to_short(orig->master_version);
    master_create_time_ = Timestamp(bcd_to_bin4(to_short(orig->master_create_time_year)),
                                    bcd_to_bin2(orig->master_create_time_month),
                                    bcd_to_bin2(orig->master_create_time_day),
                                    bcd_to_bin2(orig->master_create_time_hour),
                                    bcd_to_bin2(orig->master_create_time_minute),
                                    0.0);

    format_version_ = to_short(orig->format_version);
    format_create_time_ = Timestamp(bcd_to_bin4(to_short(orig->format_create_time_year)),
                                    bcd_to_bin2(orig->format_create_time_month),
                                    bcd_to_bin2(orig->format_create_time_day),
                                    bcd_to_bin2(orig->format_create_time_hour),
                                    bcd_to_bin2(orig->format_create_time_minute),
                                    0.0);

    master_record_count_ = to_short(orig->master_record_count);
//    right_display_ = to_short(orig->right_display);
//    left_display_ = to_short(orig->left_display);
//    right_display_sp_ = to_short(orig->right_display_sp);
//    left_display_sp_ = to_short(orig->left_display_sp);
    unsigned short account_n = 0;
    for(int i = 0; i < ACCOUNT_NUM; ++i) 
    { button_display_order_[i] = to_short((orig->button_display_order) + i * 2);
      if(button_display_order_[i] != 0) account_n++;
    }
    account_number_ = account_n;

//    unsigned short account_sp_n = 0;
//    for(int j = 0; j < ACCOUNT_NUM; ++j) 
//    { button_display_order_sp_[j] = to_short((orig->button_display_order_sp) + j * 2);
//      if(button_display_order_sp_[j] != 0) account_sp_n++;
//    }
//    account_number_sp_ = account_sp_n;


    data += sizeof COMDATA;
    ret = true;
  }
  catch (...) {}
  return ret;
}


//共通ヘッダ
typedef struct _ComPrefix {
  unsigned char station_setting_code[4];
  unsigned char delivery_year[2];
  unsigned char delivery_month;
  unsigned char delivery_day;
  unsigned char delivery_hour;
  unsigned char delivery_minute;
  unsigned char newdata_offset[4];
  unsigned char curdata_offset[4];
} COMPREFIX;

inline ComPrefix::ComPrefix()
: station_setting_code_(0),data_delivery_time_(0),newdata_offset_(0),curdata_offset_(0)
{}

inline ComPrefix::~ComPrefix(){}

inline bool ComPrefix::internalize(const char*& data)
{
  bool ret = false;
  try {
    char buf[sizeof COMPREFIX];
    memset(buf, 0, sizeof buf);
    memcpy(buf,data,sizeof buf);
    COMPREFIX* orig = reinterpret_cast<COMPREFIX*>(buf);
    station_setting_code_ = bcd_to_bin8(to_long(orig->station_setting_code));
    data_delivery_time_ = Timestamp(bcd_to_bin4(to_short(orig->delivery_year)),
                                    bcd_to_bin2(orig->delivery_month),
                                    bcd_to_bin2(orig->delivery_day),
                                    bcd_to_bin2(orig->delivery_hour),
                                    bcd_to_bin2(orig->delivery_minute),
                                    0.0);
    newdata_offset_ = to_long(orig->newdata_offset);
    curdata_offset_ = to_long(orig->curdata_offset);

    data += sizeof COMPREFIX;

    bool ret2 = true;
    for(int data_num = 0; data_num < MASTER_NUM; ++data_num){
      ret2 &= comdata_[data_num].internalize(data);
    }
    if(ret2){ ret = true; }
  }
  catch (...) {}
  return ret;
}


//企画券券種情報
typedef struct _TicketData {
  unsigned char extra_ticket_no[2];
  unsigned char issue_data_kind_no[2];
  unsigned char issue_data_no[2];
  unsigned char refund_data_kind_no[2];
  unsigned char refund_data_no[2];
//  unsigned char item_code[4];
  unsigned char reserved1[4];
  unsigned char reserved2[2];
//  unsigned char start_station_code[2];
//  unsigned char start_company_code[2];
  unsigned char reserved3[2];
  unsigned char reserved4[2];
  unsigned char reserved5[12];
//  unsigned char junction_station_code1[2];
//  unsigned char junction_company_code1[2];
  unsigned char reserved6[2];
  unsigned char reserved7[2];
  unsigned char reserved8[12];
//  unsigned char junction_station_code2[2];
//  unsigned char junction_company_code2[2];
  unsigned char reserved9[2];
  unsigned char reserved10[2];
  unsigned char reserved11[12];
//  unsigned char junction_station_code3[2];
//  unsigned char junction_company_code3[2];
  unsigned char reserved12[2];
  unsigned char reserved13[2];
  unsigned char reserved14[12];
//  unsigned char arrive_station_code[2];
//  unsigned char arrive_company_code[2];
  unsigned char reserved15[2];
  unsigned char reserved16[2];
  unsigned char reserved17[12];
  unsigned char reserved18[16];
  unsigned char issue_amount_adult[2];
  unsigned char issue_amount_child[2];
  unsigned char issue_amount_discount_adult[2];
  unsigned char issue_amount_discount_child[2];
  unsigned char division_amount_adult[2];
  unsigned char division_amount_child[2];
  unsigned char reserved19[4];
  unsigned char other_company_amount_adult[2];
  unsigned char other_company_amount_child[2];
  unsigned char other_company_amount_dis_adult[2];
  unsigned char other_company_amount_dis_child[2];
  unsigned char ticketface1_fare_adult[2];
  unsigned char ticketface1_fare_child[2];
  unsigned char ticketface2_fare_adult[2];
  unsigned char ticketface2_fare_child[2];
  unsigned char ticketface3_fare_adult[2];
  unsigned char ticketface3_fare_child[2];
  unsigned char ticketface4_fare_adult[2];
  unsigned char ticketface4_fare_child[2];
  unsigned char reserved20[4];
  unsigned char issue_num[2];
  unsigned char issue_start_date_year[2];
  unsigned char issue_start_date_month;
  unsigned char issue_start_date_day;
  unsigned char issue_end_date_year[2];
  unsigned char issue_end_date_month;
  unsigned char issue_end_date_day;
  unsigned char pre_issue_start_date_year[2];
  unsigned char pre_issue_start_date_month;
  unsigned char pre_issue_start_date_day;
//  unsigned char v_valid_date_count[2];
  unsigned char cm_valid_date_count[2];
  unsigned char reserved21[2];
  unsigned char event_start_date_year[2];
  unsigned char event_start_date_month;
  unsigned char event_start_date_day;
  unsigned char event_end_date_year[2];
  unsigned char event_end_date_month;
  unsigned char event_end_date_day;
//  unsigned char v_issue_permit;
//  unsigned char v_refund_permit;
  unsigned char cm_issue_permit;
  unsigned char cm_refund_permit;
  unsigned char reserved22;
  unsigned char reserved23;
  unsigned char is_holiday[2];
  unsigned char limit_weekday_set_start_hour;
  unsigned char limit_weekday_set_start_minute;
  unsigned char limit_weekday_set_end_hour;
  unsigned char limit_weekday_set_end_minute;
  unsigned char limit_holiday_set_start_hour;
  unsigned char limit_holiday_set_start_minute;
  unsigned char limit_holiday_set_end_hour;
  unsigned char limit_holiday_set_end_minute;
  unsigned char is_fare_change;
  unsigned char reserved24[9];
} TICKETDATA;

inline TicketData::TicketData()
: extra_ticket_no_(0),issue_data_kind_no_(0),issue_data_no_(0),refund_data_kind_no_(0),refund_data_no_(0),
//item_code_(0),start_station_code_(0),start_company_code_(0),junction_station_code1_(0),junction_company_code1_(0),
//junction_station_code2_(0),junction_company_code2_(0),junction_station_code3_(0),junction_company_code3_(0),
//arrive_station_code_(0),arrive_company_code_(0),issue_amount_adult_(0),issue_amount_child_(0),
issue_amount_adult_(0),issue_amount_child_(0),
issue_amount_discount_adult_(0),issue_amount_discount_child_(0),division_amount_adult_(0),division_amount_child_(0),
other_company_amount_adult_(0),other_company_amount_child_(0),other_company_amount_dis_adult_(0),other_company_amount_dis_child_(0),
ticketface1_fare_adult_(0),ticketface1_fare_child_(0),
ticketface2_fare_adult_(0),ticketface2_fare_child_(0),
ticketface3_fare_adult_(0),ticketface3_fare_child_(0),
ticketface4_fare_adult_(0),ticketface4_fare_child_(0),issue_num_(0),issue_start_date_(0),issue_end_date_(0),is_indefinite_(false),
//pre_issue_start_date_(0),is_pre_issue_(false),v_valid_date_count_(0),cm_valid_date_count_(0),event_start_date_(0),event_end_date_(0),
pre_issue_start_date_(0),is_pre_issue_(false),cm_valid_date_count_(0),event_start_date_(0),event_end_date_(0),
//v_issue_permit_(false),v_refund_permit_(false),cm_issue_permit_(false),cm_refund_permit_(false),is_holiday_(false),
cm_issue_permit_(false),cm_refund_permit_(false),is_holiday_(false),
limit_weekday_set_start_hour_(0),limit_weekday_set_start_minute_(0),limit_weekday_set_end_hour_(0),limit_weekday_set_end_minute_(0),
limit_holiday_set_start_hour_(0),limit_holiday_set_start_minute_(0),limit_holiday_set_end_hour_(0),limit_holiday_set_end_minute_(0),
is_fare_change_(false),button_index_(0)
{}

inline TicketData::~TicketData(){}

inline bool TicketData::internalize(const char*& data)
{
  bool ret = false;
  try {
    char buf[sizeof TICKETDATA];
    memset(buf, 0, sizeof buf);
    memcpy(buf,data,sizeof buf);
    TICKETDATA* orig = reinterpret_cast<TICKETDATA*>(buf);

    extra_ticket_no_ = to_short(orig->extra_ticket_no);
    issue_data_kind_no_ = to_short(orig->issue_data_kind_no);
    issue_data_no_ = to_short(orig->issue_data_no);
    refund_data_kind_no_ = to_short(orig->refund_data_kind_no);
    refund_data_no_ = to_short(orig->refund_data_no);
//    item_code_ = to_long(orig->item_code);
//    start_station_code_ = to_short(orig->start_station_code);
//    start_company_code_ = to_short(orig->start_company_code);
//    junction_station_code1_ = to_short(orig->junction_station_code1);
//    junction_company_code1_ = to_short(orig->junction_company_code1);
//    junction_station_code2_ = to_short(orig->junction_station_code2);
//    junction_company_code2_ = to_short(orig->junction_company_code2);
//    junction_station_code3_ = to_short(orig->junction_station_code3);
//    junction_company_code3_ = to_short(orig->junction_company_code3);
//    arrive_station_code_ = to_short(orig->arrive_station_code);
//    arrive_company_code_ = to_short(orig->arrive_company_code);
    issue_amount_adult_ = to_short(orig->issue_amount_adult);
    issue_amount_child_ = to_short(orig->issue_amount_child);
    issue_amount_discount_adult_ = to_short(orig->issue_amount_discount_adult);
    issue_amount_discount_child_ = to_short(orig->issue_amount_discount_child);
    division_amount_adult_ = to_short(orig->division_amount_adult);
    division_amount_child_ = to_short(orig->division_amount_child);
    other_company_amount_adult_ = to_short(orig->other_company_amount_adult);
    other_company_amount_child_ = to_short(orig->other_company_amount_child);
    other_company_amount_dis_adult_ = to_short(orig->other_company_amount_dis_adult);
    other_company_amount_dis_child_ = to_short(orig->other_company_amount_dis_child);
    ticketface1_fare_adult_ = to_short(orig->ticketface1_fare_adult);
    ticketface1_fare_child_ = to_short(orig->ticketface1_fare_child);
    ticketface2_fare_adult_ = to_short(orig->ticketface2_fare_adult);
    ticketface2_fare_child_ = to_short(orig->ticketface2_fare_child);
    ticketface3_fare_adult_ = to_short(orig->ticketface3_fare_adult);
    ticketface3_fare_child_ = to_short(orig->ticketface3_fare_child);
    ticketface4_fare_adult_ = to_short(orig->ticketface4_fare_adult);
    ticketface4_fare_child_ = to_short(orig->ticketface4_fare_child);
    issue_num_ = to_short(orig->issue_num);
    issue_start_date_ = Timestamp(bcd_to_bin4(to_short(orig->issue_start_date_year)),
                                    bcd_to_bin2(orig->issue_start_date_month),
                                    bcd_to_bin2(orig->issue_start_date_day),
                                    0,
                                    0,
                                    0.0);

    unsigned short issue_end_date_year = bcd_to_bin4(to_short(orig->issue_end_date_year));
    unsigned short issue_end_date_month = bcd_to_bin2(orig->issue_end_date_month);
    unsigned short issue_end_date_day = bcd_to_bin2(orig->issue_end_date_day);
    if(issue_end_date_year == 9999 && issue_end_date_month == 99 && issue_end_date_day == 99) {
      // 発売終了年月日「99999999」無期限とする
      is_indefinite_ = true;
      issue_end_date_ = Timestamp(0,
                                  0,
                                  0,
                                  0,
                                  0,
                                  0.0);
    } else {
      is_indefinite_ = false;
      issue_end_date_ = Timestamp(issue_end_date_year,
                                  issue_end_date_month,
                                  issue_end_date_day,
                                  0,
                                  0,
                                  0.0);
    }

    unsigned short pre_issue_start_date_year = bcd_to_bin4(to_short(orig->pre_issue_start_date_year));
    unsigned short pre_issue_start_date_month = bcd_to_bin2(orig->pre_issue_start_date_month);
    unsigned short pre_issue_start_date_day = bcd_to_bin2(orig->pre_issue_start_date_day);
    if(pre_issue_start_date_year == 9999 && pre_issue_start_date_month == 99 && pre_issue_start_date_day == 99) {
      // 前売開始年月日「99999999」前売なしとする
      is_pre_issue_ = false;
      pre_issue_start_date_ = Timestamp(0,
                                        0,
                                        0,
                                        0,
                                        0,
                                        0.0);
    } else {
      is_pre_issue_ = true;
      pre_issue_start_date_ = Timestamp(pre_issue_start_date_year,
                                        pre_issue_start_date_month,
                                        pre_issue_start_date_day,
                                        0,
                                        0,
                                        0.0);
    }

//    v_valid_date_count_ = to_short(orig->v_valid_date_count);
    cm_valid_date_count_ = to_short(orig->cm_valid_date_count);
    event_start_date_ = Timestamp(bcd_to_bin4(to_short(orig->event_start_date_year)),
                                    bcd_to_bin2(orig->event_start_date_month),
                                    bcd_to_bin2(orig->event_start_date_day),
                                    0,
                                    0,
                                    0.0);
    event_end_date_ = Timestamp(bcd_to_bin4(to_short(orig->event_end_date_year)),
                                    bcd_to_bin2(orig->event_end_date_month),
                                    bcd_to_bin2(orig->event_end_date_day),
                                    0,
                                    0,
                                    0.0);
//    // 多機能券売機発売可否フラグ 0:可,1:不可,それ以外:不可
//    if(orig->v_issue_permit == 0) {
//      v_issue_permit_ = true;
//    } else if(orig->v_issue_permit == 1) {
//      v_issue_permit_ = false;
//    }
//    // 多機能券売機払戻可否フラグ 0:不可,1:可,それ以外:不可
//    if(orig->v_refund_permit == 0) {
//      v_refund_permit_ = false;
//    } else if(orig->v_refund_permit == 1) {
//      v_refund_permit_ = true;
//    }
    // 窓処発売可否フラグ 0:可,1:不可,それ以外:不可
    if(orig->cm_issue_permit == 0) {
      cm_issue_permit_ = true;
    } else if(orig->cm_issue_permit == 1) {
//      cm_issue_permit_ = false;
      cm_issue_permit_ = true; // 今回未使用 常にtrueをセット
    }
    // 窓処払戻可否フラグ 0:不可,1:可,それ以外:不可
    if(orig->cm_refund_permit == 0) {
//      cm_refund_permit_ = false; 
      cm_refund_permit_ = true; // 今回未使用 常にtrueをセット
    } else if(orig->cm_refund_permit == 1) {
      cm_refund_permit_ = true;
    }
    // 土休日フラグ 0:通常日,1:土休日,それ以外:通常日
    if(to_short(orig->is_holiday) == 0) {
      is_holiday_ = false;
    } else if(to_short(orig->is_holiday) == 1) {
      is_holiday_ = true;
    }

    limit_weekday_set_start_hour_    = bcd_to_bin2(orig->limit_weekday_set_start_hour);    // 企画券制限設定（平日）開始時
    limit_weekday_set_start_minute_  = bcd_to_bin2(orig->limit_weekday_set_start_minute);  // 企画券制限設定（平日）開始分
    limit_weekday_set_end_hour_      = bcd_to_bin2(orig->limit_weekday_set_end_hour);      // 企画券制限設定（平日）終了時
    limit_weekday_set_end_minute_    = bcd_to_bin2(orig->limit_weekday_set_end_minute);    // 企画券制限設定（平日）終了分
    limit_holiday_set_start_hour_    = bcd_to_bin2(orig->limit_holiday_set_start_hour);    // 企画券制限設定（休日）開始時
    limit_holiday_set_start_minute_  = bcd_to_bin2(orig->limit_holiday_set_start_minute);  // 企画券制限設定（休日）開始分
    limit_holiday_set_end_hour_      = bcd_to_bin2(orig->limit_holiday_set_end_hour);      // 企画券制限設定（休日）終了時
    limit_holiday_set_end_minute_    = bcd_to_bin2(orig->limit_holiday_set_end_minute);    // 企画券制限設定（休日）終了分

    // 運改マークフラグ 0:不可,1:可,それ以外:不可
    if(orig->is_fare_change == 0) {
      is_fare_change_ = false;
    } else if(orig->is_fare_change == 1) {
      is_fare_change_ = true;
    }

    data += sizeof TICKETDATA;
    ret = true;
  }
  catch (...) {}
  return ret;
}

// 企画券マスタ
inline ExtraTicketMasterData::ExtraTicketMasterData():generation_(0), ticketnum_(0) {}

inline ExtraTicketMasterData::~ExtraTicketMasterData(){}

inline bool ExtraTicketMasterData::internalize(const char*& data, int generation, int ticketnum)
{
  bool ret = false;
  try {
    generation_ = generation;
    ticketnum_ = ticketnum;
    for(int i = 0; i < TICKET_NUM; ++i)
    {
      if(i >= ticketnum_) {data = data + sizeof TICKETDATA; continue; }
      if(!ticket_data_[i].internalize(data)) { break;}
      if(i == ticketnum_ - 1) { ret = true;}
    }
  } catch(...){}
  return ret;
}


//企画券グループテーブル
typedef struct _GroupTable {
  unsigned char no[2];
  unsigned char name[40];
//  unsigned char attribute[2];
  unsigned char reserved1[2];
  unsigned char ticket1_no[2];
  unsigned char ticket2_no[2];
  unsigned char ticket3_no[2];
  unsigned char ticket4_no[2];
  char v_button_word_jp_1[29];
  char v_button_word_jp_2[29];
  char v_button_word_en_1[26];
  char v_button_word_en_2[26];
  char cm_button_word_jp_1[23];
  char cm_button_word_jp_2[23];
  unsigned char reserved2[60];
} GROUPTABLE;

inline ExtraTicketGroupTable::ExtraTicketGroupTable()
: group_table_record_num_(0) {}

inline ExtraTicketGroupTable::~ExtraTicketGroupTable(){}

inline bool ExtraTicketGroupTable::internalize(const char*& data)
{
  bool ret = false;
  try {
    ExtraTicketUtils etu;
    group_data_.clear();
    group_table_record_num_ = to_short((unsigned char*)data);
    data += 2;
    for(int i = 0; i < group_table_record_num_; ++i) {
      char buf[sizeof GROUPTABLE];
      memset(buf, 0, sizeof buf);
      memcpy(buf,data,sizeof buf);
      GROUPTABLE* orig = reinterpret_cast<GROUPTABLE*>(buf);
      GroupData groupdata;
      groupdata.no = to_short(orig->no);     //   企画券No
      char name_buff[(sizeof orig->name) + 2];
      memset(name_buff, 0, sizeof name_buff);
      memcpy(name_buff,orig->name,sizeof orig->name);
      groupdata.name = etu.string_trim(string(name_buff));      //   企画券名称
//      groupdata.attribute = to_short(orig->attribute); //  属性
      groupdata.ticket1_no = to_short(orig->ticket1_no); // 一枚目フォーマットNo
      groupdata.ticket2_no = to_short(orig->ticket2_no); // 二枚目フォーマットNo
      groupdata.ticket3_no = to_short(orig->ticket3_no); // 三枚目フォーマットNo
      groupdata.ticket4_no = to_short(orig->ticket4_no); // 四枚目フォーマットNo
      groupdata.v_button_word_jp_1 = string(orig->v_button_word_jp_1, sizeof orig->v_button_word_jp_1);
      groupdata.v_button_word_jp_2 = string(orig->v_button_word_jp_2, sizeof orig->v_button_word_jp_2);
      groupdata.v_button_word_en_1 = string(orig->v_button_word_en_1, sizeof orig->v_button_word_en_1);
      groupdata.v_button_word_en_2 = string(orig->v_button_word_en_2, sizeof orig->v_button_word_en_2);
      groupdata.cm_button_word_jp_1 = string(orig->cm_button_word_jp_1, sizeof orig->cm_button_word_jp_1);
      groupdata.cm_button_word_jp_2 = string(orig->cm_button_word_jp_2, sizeof orig->cm_button_word_jp_2);
      group_data_.push_back(groupdata);
      data += sizeof GROUPTABLE;
    }
    ret = true;
  }
  catch (...) {}
  return ret;
}


//企画券フォーマットテーブル
typedef struct _FormatTable {
  unsigned char no[2];
  unsigned char name[40];
  unsigned char ticket_kind[2];
//  unsigned char attribute[2];
  unsigned char reserved1[2];
  unsigned char is_amount_print[2];
  unsigned char start_station[2];
  unsigned char is_numbering_print[2];
  unsigned char reserved2[2];
  unsigned char encode_data[56];
  unsigned char reserved3[16];
} FORMATTABLE;

typedef struct _FMBigstEncode {
  unsigned char ticket_kind3[2];
  unsigned char start1[2];
  unsigned char start2[2];
  unsigned char start3[2];
  unsigned char arrive1[2];
  unsigned char arrive2[2];
  unsigned char arrive3[2];
  unsigned char via_table_no[2];
  unsigned char via1[2];
  unsigned char via2[2];
  unsigned char via3[2];
  unsigned char via4[2];
  unsigned char via5[2];
  unsigned char via6[2];
  unsigned char via7[2];
  unsigned char via8[2];
  unsigned char via9[2];
  unsigned char via10[2];
  unsigned char free_area_code[2];
  unsigned char ticket_kind4_detail[2];
  unsigned char roundtrip_info;
  unsigned char ticket_type;
  unsigned char item_code[4];
  unsigned char discount_code;
  unsigned char reserved[9];
} FMBIGSTENCODE;

typedef struct _FMPassEncode {
  unsigned char ticket_kind3[2];
  unsigned char start[2];
  unsigned char arrive[2];
  unsigned char via_table_no[2];
  unsigned char via1[2];
  unsigned char via2[2];
  unsigned char via3[2];
  unsigned char via4[2];
  unsigned char via5[2];
  unsigned char via6[2];
  unsigned char via7[2];
  unsigned char via8[2];
  unsigned char doubleness_bit;
  unsigned char reserved[31];
} FMPASSENCODE;


inline ExtraTicketFormatTable::ExtraTicketFormatTable()
: format_table_record_num_(0) {}

inline ExtraTicketFormatTable::~ExtraTicketFormatTable(){}

inline bool ExtraTicketFormatTable::internalize(const char*& data)
{
  bool ret = false;
  try {
    ExtraTicketUtils etu;
    format_data_.clear();
    format_table_record_num_ = to_short((unsigned char*)data);
    data += 2;
    for(int i = 0; i < format_table_record_num_; ++i) {
      char buf[sizeof FORMATTABLE];
      memset(buf, 0, sizeof buf);
      memcpy(buf,data,sizeof buf);
      FORMATTABLE* orig = reinterpret_cast<FORMATTABLE*>(buf);
      FormatData formatdata;
      formatdata.no = to_short(orig->no);     //   企画券No
      char name_buff[(sizeof orig->name) + 2];
      memset(name_buff, 0, sizeof name_buff);
      memcpy(name_buff,orig->name,sizeof orig->name);
      formatdata.name = etu.string_trim(string(name_buff));      //   企画券名称
      formatdata.ticket_kind = to_short(orig->ticket_kind); //  券種
//      formatdata.attribute = to_short(orig->attribute); //  属性
      if(to_short(orig->is_amount_print) == 0) {
        formatdata.is_amount_print = true;
      } else if(to_short(orig->is_amount_print) == 1) {
        formatdata.is_amount_print = false;
      }
      formatdata.start_station.line = orig->start_station[0];
      formatdata.start_station.station = orig->start_station[1];
      if(to_short(orig->is_numbering_print) == 0) {
        formatdata.is_numbering_print = false;
      } else if(to_short(orig->is_numbering_print) == 1) {
        formatdata.is_numbering_print = true;
      }
      if(formatdata.ticket_kind == ENCODE_PASS) {
        char buff_ps[sizeof FMPASSENCODE];
        memset(buff_ps, 0, sizeof buff_ps);
        memcpy(buff_ps,orig->encode_data,sizeof buff_ps);
        FMPASSENCODE* orig_ps = reinterpret_cast<FMPASSENCODE*>(buff_ps);
        formatdata.fmpass_encode.ticket_kind3 = to_short(orig_ps->ticket_kind3);
        formatdata.fmpass_encode.start.line = orig_ps->start[0];
        formatdata.fmpass_encode.start.station = orig_ps->start[1];
        formatdata.fmpass_encode.arrive.line = orig_ps->arrive[0];
        formatdata.fmpass_encode.arrive.station = orig_ps->arrive[1];
        formatdata.fmpass_encode.via_table_no = to_short(orig_ps->via_table_no);
        formatdata.fmpass_encode.via1.line = orig_ps->via1[0];
        formatdata.fmpass_encode.via1.station = orig_ps->via1[1];
        formatdata.fmpass_encode.via2.line = orig_ps->via2[0];
        formatdata.fmpass_encode.via2.station = orig_ps->via2[1];
        formatdata.fmpass_encode.via3.line = orig_ps->via3[0];
        formatdata.fmpass_encode.via3.station = orig_ps->via3[1];
        formatdata.fmpass_encode.via4.line = orig_ps->via4[0];
        formatdata.fmpass_encode.via4.station = orig_ps->via4[1];
        formatdata.fmpass_encode.via5.line = orig_ps->via5[0];
        formatdata.fmpass_encode.via5.station = orig_ps->via5[1];
        formatdata.fmpass_encode.via6.line = orig_ps->via6[0];
        formatdata.fmpass_encode.via6.station = orig_ps->via6[1];
        formatdata.fmpass_encode.via7.line = orig_ps->via7[0];
        formatdata.fmpass_encode.via7.station = orig_ps->via7[1];
        formatdata.fmpass_encode.via8.line = orig_ps->via8[0];
        formatdata.fmpass_encode.via8.station = orig_ps->via8[1];
        if(orig_ps->doubleness_bit == 0) {
          formatdata.fmpass_encode.doubleness_bit = false;
        } else if(orig_ps->doubleness_bit == 1) {
          formatdata.fmpass_encode.doubleness_bit = true;
        }
      } else if(formatdata.ticket_kind == ENCODE_FIX) {
        char buff_bg[sizeof FMBIGSTENCODE];
        memset(buff_bg, 0, sizeof buff_bg);
        memcpy(buff_bg,orig->encode_data,sizeof buff_bg);
        FMBIGSTENCODE* orig_bg = reinterpret_cast<FMBIGSTENCODE*>(buff_bg);
        formatdata.fmbigst_encode.ticket_kind3 = to_short(orig_bg->ticket_kind3);
        formatdata.fmbigst_encode.start1.line = orig_bg->start1[0];
        formatdata.fmbigst_encode.start1.station = orig_bg->start1[1];
        formatdata.fmbigst_encode.start2.line = orig_bg->start2[0];
        formatdata.fmbigst_encode.start2.station = orig_bg->start2[1];
        formatdata.fmbigst_encode.start3.line = orig_bg->start3[0];
        formatdata.fmbigst_encode.start3.station = orig_bg->start3[1];
        formatdata.fmbigst_encode.arrive1.line = orig_bg->arrive1[0];
        formatdata.fmbigst_encode.arrive1.station = orig_bg->arrive1[1];
        formatdata.fmbigst_encode.arrive2.line = orig_bg->arrive2[0];
        formatdata.fmbigst_encode.arrive2.station = orig_bg->arrive2[1];
        formatdata.fmbigst_encode.arrive3.line = orig_bg->arrive3[0];
        formatdata.fmbigst_encode.arrive3.station = orig_bg->arrive3[1];
        formatdata.fmbigst_encode.via_table_no = to_short(orig_bg->via_table_no);
        formatdata.fmbigst_encode.via1.line = orig_bg->via1[0];
        formatdata.fmbigst_encode.via1.station = orig_bg->via1[1];
        formatdata.fmbigst_encode.via2.line = orig_bg->via2[0];
        formatdata.fmbigst_encode.via2.station = orig_bg->via2[1];
        formatdata.fmbigst_encode.via3.line = orig_bg->via3[0];
        formatdata.fmbigst_encode.via3.station = orig_bg->via3[1];
        formatdata.fmbigst_encode.via4.line = orig_bg->via4[0];
        formatdata.fmbigst_encode.via4.station = orig_bg->via4[1];
        formatdata.fmbigst_encode.via5.line = orig_bg->via5[0];
        formatdata.fmbigst_encode.via5.station = orig_bg->via5[1];
        formatdata.fmbigst_encode.via6.line = orig_bg->via6[0];
        formatdata.fmbigst_encode.via6.station = orig_bg->via6[1];
        formatdata.fmbigst_encode.via7.line = orig_bg->via7[0];
        formatdata.fmbigst_encode.via7.station = orig_bg->via7[1];
        formatdata.fmbigst_encode.via8.line = orig_bg->via8[0];
        formatdata.fmbigst_encode.via8.station = orig_bg->via8[1];
        formatdata.fmbigst_encode.via9.line = orig_bg->via9[0];
        formatdata.fmbigst_encode.via9.station = orig_bg->via9[1];
        formatdata.fmbigst_encode.via10.line = orig_bg->via10[0];
        formatdata.fmbigst_encode.via10.station = orig_bg->via10[1];
        formatdata.fmbigst_encode.free_area_code.line = orig_bg->free_area_code[0];
        formatdata.fmbigst_encode.free_area_code.station = orig_bg->free_area_code[1];

        formatdata.fmbigst_encode.ticket_kind4_detail = to_short(orig_bg->ticket_kind4_detail);
        formatdata.fmbigst_encode.roundtrip_info = orig_bg->roundtrip_info;
        formatdata.fmbigst_encode.ticket_type = orig_bg->ticket_type;
        formatdata.fmbigst_encode.item_code = to_long(orig_bg->item_code);
        formatdata.fmbigst_encode.discount_code = orig_bg->discount_code;
      }

      format_data_.push_back(formatdata);
      data += sizeof FORMATTABLE;
    }
    ret = true;
  }
  catch (...) {}
  return ret;
}


////企画券経由パターンテーブル経由８定期券用
//typedef struct _Via8Info {
//  unsigned char line;
//  unsigned char station_lower;
//  unsigned char station_upper;
//  unsigned char via_num;
//  unsigned char via1[2];
//  unsigned char via2[2];
//  unsigned char via3[2];
//  unsigned char via4[2];
//  unsigned char via5[2];
//  unsigned char via6[2];
//  unsigned char via7[2];
//  unsigned char via8[2];
//} VIA8INFO;
//
//inline ExtraTicketVia8Table::ExtraTicketVia8Table()
//: via8_table_record_num_(0) {}
//
//inline ExtraTicketVia8Table::~ExtraTicketVia8Table(){}
//
//inline bool ExtraTicketVia8Table::internalize(const char*& data)
//{
//  bool ret = false;
//  try {
//    via8_data_.clear();
//    via8_table_record_num_ = to_short((unsigned char*)data);
//    data += 2;
//    for(int ii = 0; ii < via8_table_record_num_; ++ii) {
//      // 経由テーブルNo
//      data += 2;
//      // オフセット
//      data += 2;
//    }
//    for(int i = 0; i < via8_table_record_num_; ++i) {
//      Via8Data via8data;
//      // 経由テーブルNo
//      via8data.via_table_no = to_short((unsigned char*)data);
//      data += 2;
//      // 経由情報数
//      via8data.via_info_num = to_short((unsigned char*)data);
//      data += 2;
//      via8data.via8_info.clear();
//      for (int j = 0; j < via8data.via_info_num; ++j) {
//        char buf[sizeof VIA8INFO];
//        memset(buf, 0, sizeof buf);
//        memcpy(buf,data,sizeof buf);
//        VIA8INFO* orig = reinterpret_cast<VIA8INFO*>(buf);
//        Via8Info via8info;
//        via8info.line = orig->line;
//        via8info.station_lower = orig->station_lower;
//        via8info.station_upper = orig->station_upper;
//        via8info.via_num = orig->via_num;
//        via8info.via1.line = orig->via1[0];
//        via8info.via1.station = orig->via1[1];
//        via8info.via2.line = orig->via2[0];
//        via8info.via2.station = orig->via2[1];
//        via8info.via3.line = orig->via3[0];
//        via8info.via3.station = orig->via3[1];
//        via8info.via4.line = orig->via4[0];
//        via8info.via4.station = orig->via4[1];
//        via8info.via5.line = orig->via5[0];
//        via8info.via5.station = orig->via5[1];
//        via8info.via6.line = orig->via6[0];
//        via8info.via6.station = orig->via6[1];
//        via8info.via7.line = orig->via7[0];
//        via8info.via7.station = orig->via7[1];
//        via8info.via8.line = orig->via8[0];
//        via8info.via8.station = orig->via8[1];
//        via8data.via8_info.push_back(via8info);
//        data += sizeof VIA8INFO;
//      }
//      via8_data_.push_back(via8data);
//    }
//    ret = true;
//  }
//  catch (...) {}
//  return ret;
//}
//
//
////企画券経由パターンテーブル経由１０大型券用
//typedef struct _Via10Info {
//  unsigned char line;
//  unsigned char station_lower;
//  unsigned char station_upper;
//  unsigned char via_num;
//  unsigned char via1[2];
//  unsigned char via2[2];
//  unsigned char via3[2];
//  unsigned char via4[2];
//  unsigned char via5[2];
//  unsigned char via6[2];
//  unsigned char via7[2];
//  unsigned char via8[2];
//  unsigned char via9[2];
//  unsigned char via10[2];
//} VIA10INFO;
//
//inline ExtraTicketVia10Table::ExtraTicketVia10Table()
//: via10_table_record_num_(0) {}
//
//inline ExtraTicketVia10Table::~ExtraTicketVia10Table(){}
//
//inline bool ExtraTicketVia10Table::internalize(const char*& data)
//{
//  bool ret = false;
//  try {
//    via10_data_.clear();
//    via10_table_record_num_ = to_short((unsigned char*)data);
//    data += 2;
//    for(int ii = 0; ii < via10_table_record_num_; ++ii) {
//      // 経由テーブルNo
//      data += 2;
//      // オフセット
//      data += 2;
//    }
//    for(int i = 0; i < via10_table_record_num_; ++i) {
//      Via10Data via10data;
//      // 経由テーブルNo
//      via10data.via_table_no = to_short((unsigned char*)data);
//      data += 2;
//      // 経由情報数
//      via10data.via_info_num = to_short((unsigned char*)data);
//      data += 2;
//      via10data.via10_info.clear();
//      for (int j = 0; j < via10data.via_info_num; ++j) {
//        char buf[sizeof VIA10INFO];
//        memset(buf, 0, sizeof buf);
//        memcpy(buf,data,sizeof buf);
//        VIA10INFO* orig = reinterpret_cast<VIA10INFO*>(buf);
//        Via10Info via10info;
//        via10info.line = orig->line;
//        via10info.station_lower = orig->station_lower;
//        via10info.station_upper = orig->station_upper;
//        via10info.via_num = orig->via_num;
//        via10info.via1.line = orig->via1[0];
//        via10info.via1.station = orig->via1[1];
//        via10info.via2.line = orig->via2[0];
//        via10info.via2.station = orig->via2[1];
//        via10info.via3.line = orig->via3[0];
//        via10info.via3.station = orig->via3[1];
//        via10info.via4.line = orig->via4[0];
//        via10info.via4.station = orig->via4[1];
//        via10info.via5.line = orig->via5[0];
//        via10info.via5.station = orig->via5[1];
//        via10info.via6.line = orig->via6[0];
//        via10info.via6.station = orig->via6[1];
//        via10info.via7.line = orig->via7[0];
//        via10info.via7.station = orig->via7[1];
//        via10info.via8.line = orig->via8[0];
//        via10info.via8.station = orig->via8[1];
//        via10info.via9.line = orig->via9[0];
//        via10info.via9.station = orig->via9[1];
//        via10info.via10.line = orig->via10[0];
//        via10info.via10.station = orig->via10[1];
//        via10data.via10_info.push_back(via10info);
//        data += sizeof VIA10INFO;
//      }
//      via10_data_.push_back(via10data);
//    }
//    ret = true;
//  }
//  catch (...) {}
//  return ret;
//}


//企画券券面フォーマットデータ
typedef struct _Illustration {
//  unsigned char is_v_print;
  unsigned char is_cm_print;
  unsigned char reserved;
  unsigned char point_x[2];
  unsigned char point_y[2];
  unsigned char width[2];
  unsigned char height[2];
  unsigned char illust_offset[4];
} ILLUSTRATION;

inline ExtraTicketFaceData::ExtraTicketFaceData()
: ticketface_data_record_num_(0) {}

inline ExtraTicketFaceData::~ExtraTicketFaceData(){}

inline bool ExtraTicketFaceData::internalize(const char*& data)
{
  bool ret = false;
  try {
    ticketface_data_.clear();
    ticketface_data_record_num_ = to_short((unsigned char*)data);
    data += 2;
    for(int i = 0; i < ticketface_data_record_num_; ++i) {
      TicketFaceData ticketfacedata;
      ticketfacedata.no = to_short((unsigned char*)data);
      data += 2;
      char format_code_buff[4];
      memset(format_code_buff, 0, sizeof format_code_buff);
      memcpy(format_code_buff,data,2);
      ticketfacedata.format_code = string(format_code_buff);
      data += 2;
      unsigned short is_numbering_print = to_short((unsigned char*)data);
      if(is_numbering_print == 0) {
        ticketfacedata.is_numbering_print = false;
      } else if(is_numbering_print == 1) {
        ticketfacedata.is_numbering_print = true;
      }
      data += 2;
      ticketfacedata.illustration_num = to_short((unsigned char*)data);
      data += 2;
      ticketfacedata.illustration_info.clear();
      for(int j = 0; j < ticketfacedata.illustration_num; ++j) {
        char buf[sizeof ILLUSTRATION];
        memset(buf, 0, sizeof buf);
        memcpy(buf,data,sizeof buf);
        ILLUSTRATION* orig = reinterpret_cast<ILLUSTRATION*>(buf);
        Illustration illust;
//        if(orig->is_v_print == 0) {
//          illust.is_v_print = false;
//        } else if(orig->is_v_print == 1) {
//          illust.is_v_print = true;
//        }
        if(orig->is_cm_print == 0) {
          illust.is_cm_print = false;
        } else if(orig->is_cm_print == 1) {
          illust.is_cm_print = true;
        }
        illust.point_x = to_short(orig->point_x);
        illust.point_y = to_short(orig->point_y);
        illust.width = to_short(orig->width);
        illust.height = to_short(orig->height);
        illust.illust_offset = to_long(orig->illust_offset);

        ticketfacedata.illustration_info.push_back(illust);
        data += sizeof ILLUSTRATION;
      }
      ticketfacedata.print_info_num = to_short((unsigned char*)data);
      data += 2;
      ticketfacedata.print_info.clear();
      for(int k = 0; k < ticketfacedata.print_info_num; ++k) {
        PrintInfo printinfo;
        printinfo.word_line_num = to_short((unsigned char*)data);
        data += 2;
        printinfo.print_word = "";
        for(int kk = 0; kk < printinfo.word_line_num; kk += 2) {
          char print_word_buff[4];
          memset(print_word_buff, 0, sizeof print_word_buff);
          memcpy(print_word_buff, data, 2);
          printinfo.print_word = printinfo.print_word + string(print_word_buff);
          data += 2;
        }
        ticketfacedata.print_info.push_back(printinfo);
      }
      ticketface_data_.push_back(ticketfacedata);
    }
    ret = true;
  }
  catch (...) {}
  return ret;
}

// 企画券フォーマットデータ情報
inline TicketFormatData::TicketFormatData()
: extra_ticket_no_(0), extra_ticket_name_(""), encode_num_(0),
  v_button_word_jp_1_(""), v_button_word_jp_2_(""), v_button_word_en_1_(""), v_button_word_en_2_(""),
  cm_button_word_jp_1_(""), cm_button_word_jp_2_("")
{}

inline TicketFormatData::~TicketFormatData(){}


// 企画券フォーマット データヘッダ
typedef struct _FormatDataCom {
  unsigned char station_code[2];
  unsigned char operate_date_year[2];
  unsigned char operate_date_month;
  unsigned char operate_date_day;
  unsigned char record_num[2];
} FORMATDATACOM;

typedef struct _FormatDataHead {
  unsigned char identify_word[20];
  unsigned char effective_start_date_year[2];
  unsigned char effective_start_date_month;
  unsigned char effective_start_date_day;
  unsigned char tool_version[2];
  unsigned char reserved1[22];
  unsigned char format_version[2];
  unsigned char format_create_time_year[2];
  unsigned char format_create_time_month;
  unsigned char format_create_time_day;
  unsigned char format_create_time_hour;
  unsigned char format_create_time_minute;
//  unsigned char author[4];
  unsigned char reserved2[4];
  unsigned char reserved3[4];
} FORMATDATAHEAD;

// 企画券フォーマット インデックステーブル
typedef struct _FormatDataIndex {
  unsigned char index_group_table[4];
  unsigned char index_format_table[4];
//  unsigned char index_via8_table[4];
//  unsigned char index_via10_table[4];
  unsigned char reserved1[4];
  unsigned char reserved2[4];
  unsigned char index_ticketface_data[4];
  unsigned char reserved3[12];
} FORMATDATAINDEX;


// 企画券フォーマット
inline ExtraTicketFormatData::ExtraTicketFormatData()
: station_code_(0), operate_date_(0), record_num_(0), effective_start_date_(0), tool_version_(0), format_version_(0), format_create_time_(0), //author_(0),
//  index_group_table_(0), index_format_table_(0), index_via8_table_(0), index_via10_table_(0), index_ticketface_data_(0), generation_(0)
  index_group_table_(0), index_format_table_(0), index_ticketface_data_(0), generation_(0)
{ 
  memset(identify_word_, 0, sizeof identify_word_);
}

inline ExtraTicketFormatData::~ExtraTicketFormatData(){}

inline bool ExtraTicketFormatData::internalize(const char*& data, int generation)
{
  bool ret = false;
  try {
    generation_ = generation;
    const char* data_backup = data;
    char buf_com[sizeof FORMATDATACOM];
    memset(buf_com, 0, sizeof buf_com);
    memcpy(buf_com,data,sizeof buf_com);
    FORMATDATACOM* orig_com = reinterpret_cast<FORMATDATACOM*>(buf_com);
    station_code_ = to_short(orig_com->station_code);
    operate_date_ = Timestamp(bcd_to_bin4(to_short(orig_com->operate_date_year)),
                                    bcd_to_bin2(orig_com->operate_date_month),
                                    bcd_to_bin2(orig_com->operate_date_day),
                                    0,
                                    0,
                                    0.0);
    record_num_ = to_short(orig_com->record_num);
    data += sizeof FORMATDATACOM;

    char buf_head[sizeof FORMATDATAHEAD];
    memset(buf_head, 0, sizeof buf_head);
    memcpy(buf_head,data,sizeof buf_head);
    FORMATDATAHEAD* orig_head = reinterpret_cast<FORMATDATAHEAD*>(buf_head);
    memcpy(identify_word_, orig_head->identify_word, sizeof identify_word_);
    effective_start_date_ = Timestamp(bcd_to_bin4(to_short(orig_head->effective_start_date_year)),
                                    bcd_to_bin2(orig_head->effective_start_date_month),
                                    bcd_to_bin2(orig_head->effective_start_date_day),
                                    0,
                                    0,
                                    0.0);
    tool_version_ = to_short(orig_head->tool_version);
    format_version_ = to_short(orig_head->format_version);
    format_create_time_ = Timestamp(bcd_to_bin4(to_short(orig_head->format_create_time_year)),
                                    bcd_to_bin2(orig_head->format_create_time_month),
                                    bcd_to_bin2(orig_head->format_create_time_day),
                                    bcd_to_bin2(orig_head->format_create_time_hour),
                                    bcd_to_bin2(orig_head->format_create_time_minute),
                                    0.0);
//    author_ = to_long(orig_head->author);
    data += sizeof FORMATDATAHEAD;

    char buf_index[sizeof FORMATDATAINDEX];
    memset(buf_index, 0, sizeof buf_index);
    memcpy(buf_index,data,sizeof buf_index);
    FORMATDATAINDEX* orig_index = reinterpret_cast<FORMATDATAINDEX*>(buf_index);
    index_group_table_ = to_long(orig_index->index_group_table);
    index_format_table_ = to_long(orig_index->index_format_table);
//    index_via8_table_ = to_long(orig_index->index_via8_table);
//    index_via10_table_ = to_long(orig_index->index_via10_table);
    index_ticketface_data_ = to_long(orig_index->index_ticketface_data);
    data += sizeof FORMATDATAINDEX;

    // 券面フォーマットデータヘッダに遷移
    const char* data_ticketface_format_offset = data_backup + sizeof FORMATDATACOM + index_ticketface_data_;
    if(group_table_.internalize(data)
      && format_table_.internalize(data)
//      && via8_table_.internalize(data)
//      && via10_table_.internalize(data)
      && ticketface_format_data_.internalize(data_ticketface_format_offset)
      && ticketface_data_bmp_create(data_backup)
      && ticketface_data_file_create(data_backup)
      && ticketformat_data_set())
    {
      ret = true;
    }
  } catch(...){}
  return ret;
}

inline void ExtraTicketFormatData::station_code_change(LineCode station_code_in, LineCode& station_code_out)
{
  if(station_code_in.line == 0xFF && station_code_in.station == 0x00){
    station_code_out = my_station_code_;
  } else {
    station_code_out = station_code_in;
  }
}

inline bool ExtraTicketFormatData::ticketface_data_bmp_create(const char* data)
{
  bool ret = false;
  try {
    const char* pos = data;
    for(int i = 0; i < ticketface_format_data_.ticketface_data_record_num_ && i < ticketface_format_data_.ticketface_data_.size(); ++i) {
      // extra_ticket_1(世代)_1(フォーマットNo)
      std::string ticketface_name_head = "";
      std::ostringstream oss_ticketface_name;
      oss_ticketface_name << string(EXTRA_TICKET_FACE_NAME) << string(UNDER_BAR) 
          << generation_ << string(UNDER_BAR) << (int)ticketface_format_data_.ticketface_data_[i].no;
      ticketface_name_head = oss_ticketface_name.str();
      
      // イラスト作成
      for(int j = 0; j < ticketface_format_data_.ticketface_data_[i].illustration_num 
        && j < ticketface_format_data_.ticketface_data_[i].illustration_info.size(); ++j) {
        if(!ticketface_format_data_.ticketface_data_[i].illustration_info[j].is_cm_print) continue;
        const char* pos_image = pos + sizeof FORMATDATACOM + index_ticketface_data_ + ticketface_format_data_.ticketface_data_[i].illustration_info[j].illust_offset;
        // extra_ticket_1(世代)_1(フォーマットNo)_1(イラストNo).bmp
        std::string bmp_name = "";
        std::string bmp_name_full = "";
        std::ostringstream oss_bmp_name;
        std::ostringstream oss_bmp_name_full;
        oss_bmp_name << ticketface_name_head << string(UNDER_BAR) << j + 1 << string(BITMAP_EXT);
        bmp_name = oss_bmp_name.str();
        oss_bmp_name_full << string(EXTRA_TICKET_BMP_PATH) << bmp_name;
        bmp_name_full = oss_bmp_name_full.str();

        bool isYOKO = true;
        if(ticketface_format_data_.ticketface_data_[i].format_code == string("Ｌ")||
           ticketface_format_data_.ticketface_data_[i].format_code == string("Ｋ")){
          isYOKO = false;
        }
        // ビットマップ作成
        ETFmtBmpMaker fbm;
        if(!fbm.MakeFontBmp(bmp_name_full.c_str(), isYOKO,
          ticketface_format_data_.ticketface_data_[i].illustration_info[j].width,
          ticketface_format_data_.ticketface_data_[i].illustration_info[j].height,
          (const unsigned char*)pos_image)) return false;
        // メモリに同じ名前のビットマップを破壊する
        CbmDelBmp(bmp_name.c_str());
        // 新のビットマップ読み込み
        CbmReadBmp(EXTRA_TICKET_BMP_PATH, bmp_name.c_str());
      }
    }
    ret = true;
  } catch(...) {}
  return ret;
}


inline bool ExtraTicketFormatData::ticketface_data_file_create(const char* /*data*/)
{
  bool ret = false;
  try {
    for(int i = 0; i < ticketface_format_data_.ticketface_data_record_num_ && i < ticketface_format_data_.ticketface_data_.size(); ++i) {
      // extra_ticket_1(世代)_1(フォーマットNo)
      std::string ticketface_name_head = "";
      std::ostringstream oss_ticketface_name;
      oss_ticketface_name << string(EXTRA_TICKET_FACE_NAME) << string(UNDER_BAR) 
          << generation_ << string(UNDER_BAR) << (int)ticketface_format_data_.ticketface_data_[i].no;
      ticketface_name_head = oss_ticketface_name.str();
      
//      // extra_ticket_Ｒ（全角）ベース券面ファイル名称
      // extra_ticketＲ（全角）ベース券面ファイル名称
      std::string base_ticketface_name_full = "";
      std::ostringstream oss_base_ticketface_name_full;
      oss_base_ticketface_name_full << string(EXTRA_TICKET_FACE_PATH) << string(EXTRA_TICKET_FACE_NAME)// << string(UNDER_BAR) extra_ticket_Ｒ⇒extra_ticketＲ
          << ticketface_format_data_.ticketface_data_[i].format_code << string(TICKET_FACE_EXT);
      base_ticketface_name_full = oss_base_ticketface_name_full.str();

      // extra_ticket_1(世代)_1(フォーマットNo).tf 券面ファイル名称
      std::string ticketface_name_full = "";
      std::ostringstream oss_ticketface_name_full;
      oss_ticketface_name_full << string(EXTRA_TICKET_FACE_PATH) << ticketface_name_head << string(TICKET_FACE_EXT);
      ticketface_name_full = oss_ticketface_name_full.str();

      // 券面ファイル作成
      TicketFaceFileEdit tfe(ticketface_name_full);
      if(tfe.open(base_ticketface_name_full)) {

        // イラスト追加
        for(int j = 0; j < ticketface_format_data_.ticketface_data_[i].illustration_num 
          && j < ticketface_format_data_.ticketface_data_[i].illustration_info.size(); ++j) {
          if(!ticketface_format_data_.ticketface_data_[i].illustration_info[j].is_cm_print) continue;
          // extra_ticket_1(世代)_1(フォーマットNo)_1(イラストNo).bmp
          std::string bmp_name = "";
          std::ostringstream oss_bmp_name;
          oss_bmp_name << ticketface_name_head << string(UNDER_BAR) << j + 1 << string(BITMAP_EXT);
          bmp_name = oss_bmp_name.str();
          int dotx = (int)ticketface_format_data_.ticketface_data_[i].illustration_info[j].point_x;
          int doty = (int)ticketface_format_data_.ticketface_data_[i].illustration_info[j].point_y;
          int dotx_tf = doty;
          int doty_tf = Y_MAX - (dotx + (int)ticketface_format_data_.ticketface_data_[i].illustration_info[j].width);
          if(ticketface_format_data_.ticketface_data_[i].format_code == string("Ｌ")||
             ticketface_format_data_.ticketface_data_[i].format_code == string("Ｋ")){
              dotx_tf = dotx;
              doty_tf = doty;
          }
          if(!tfe.replaceFixedBmpField(dotx_tf, doty_tf, bmp_name)) return false;
        }
        // 注意文追加
        vector<std::string> pring_word_str;
        pring_word_str.clear();
        for(int k = 0; k < ticketface_format_data_.ticketface_data_[i].print_info_num 
          && k < ticketface_format_data_.ticketface_data_[i].print_info.size(); ++k){
          pring_word_str.push_back(ticketface_format_data_.ticketface_data_[i].print_info[k].print_word);
        }
        if(!pring_word_str.empty()) {
          if(!tfe.replaceFixedTxtField(pring_word_str)) return false;
        }
        // TFファイル保存
        if(!tfe.save()) return false;
        // TFファイル読み込み
        if(!TableReaderExtraTickHolder::getInstance().read_ticketface(ticketface_name_head.c_str())) return false;
      } else {return false;}
    }
    ret = true;
  } catch(...) {}
  return ret;
}

inline bool ExtraTicketFormatData::ticketformat_data_set()
{
  bool ret = false;
  try {
    SettingData sd;
    unsigned long station_code = sd.lget_data(SET_STATION, CODE);
    my_station_code_.line = (unsigned char)(station_code / 1000);
    my_station_code_.station = (unsigned char)(station_code % 1000);
    // 企画券フォーマットデータ情報 セット
    ticketformat_data_.clear();
    for(int i = 0; i < group_table_.group_table_record_num_ && i < group_table_.group_data_.size(); ++i){
      TicketFormatData ticketformatdata;
      ticketformatdata.extra_ticket_no_ = group_table_.group_data_[i].no;
      ticketformatdata.extra_ticket_name_ = group_table_.group_data_[i].name;
      ticketformatdata.v_button_word_jp_1_ = group_table_.group_data_[i].v_button_word_jp_1;
      ticketformatdata.v_button_word_jp_2_ = group_table_.group_data_[i].v_button_word_jp_2;
      ticketformatdata.v_button_word_en_1_ = group_table_.group_data_[i].v_button_word_en_1;
      ticketformatdata.v_button_word_en_2_ = group_table_.group_data_[i].v_button_word_en_2;
      ticketformatdata.cm_button_word_jp_1_ = group_table_.group_data_[i].cm_button_word_jp_1;
      ticketformatdata.cm_button_word_jp_2_ = group_table_.group_data_[i].cm_button_word_jp_2;
      unsigned short ticketformatno = 0;
      for(int j = 0; j < ISSUE_NUM; ++j){
        if(j == 0) ticketformatno = group_table_.group_data_[i].ticket1_no;
        else if(j == 1) ticketformatno = group_table_.group_data_[i].ticket2_no;
        else if(j == 2) ticketformatno = group_table_.group_data_[i].ticket3_no;
        else if(j == 3) ticketformatno = group_table_.group_data_[i].ticket4_no;
        if(ticketformatno != 0){
          for(int ft_i = 0; ft_i < format_table_.format_table_record_num_ && ft_i < format_table_.format_data_.size(); ++ft_i){
            if(ticketformatno == format_table_.format_data_[ft_i].no){
              // 券種
              ticketformatdata.issue_info_[j].ticket_kind = format_table_.format_data_[ft_i].ticket_kind;
              // 券面ファイル名前
              std::ostringstream ticketface_name_oss;
              ticketface_name_oss << string(EXTRA_TICKET_FACE_NAME) << string(UNDER_BAR) 
                << generation_ << string(UNDER_BAR) << (int)ticketformatno;
              ticketformatdata.issue_info_[j].ticketface_name = ticketface_name_oss.str() ;
              // 金額印字有無
              ticketformatdata.issue_info_[j].is_amount_print = format_table_.format_data_[ft_i].is_amount_print;
              // 発駅
              station_code_change(format_table_.format_data_[ft_i].start_station, ticketformatdata.issue_info_[j].start_station);
              // ナンバリング印字有無
              ticketformatdata.issue_info_[j].is_numbering_print = format_table_.format_data_[ft_i].is_numbering_print;
              // フォーマットコード ベース券面コード（Ａ～Ｌ）
              ticketformatdata.issue_info_[j].format_code = ticketface_format_data_.ticketface_data_[ft_i].format_code;
              // 定期券
              if(ticketformatdata.issue_info_[j].ticket_kind == ENCODE_PASS){
                ticketformatdata.encode_num_++;
                // 券種３
                ticketformatdata.issue_info_[j].fmpass_ticket_kind3 = format_table_.format_data_[ft_i].fmpass_encode.ticket_kind3;
                // 発駅
                station_code_change(format_table_.format_data_[ft_i].fmpass_encode.start, ticketformatdata.issue_info_[j].fmpass_start_station);
                // 着駅
                station_code_change(format_table_.format_data_[ft_i].fmpass_encode.arrive, ticketformatdata.issue_info_[j].fmpass_arrive_station);
                // 経由テーブル
                if(format_table_.format_data_[ft_i].fmpass_encode.via_table_no == 0){
                  // 経由１
                  station_code_change(format_table_.format_data_[ft_i].fmpass_encode.via1, ticketformatdata.issue_info_[j].fmpass_via_station1);
                  // 経由２
                  station_code_change(format_table_.format_data_[ft_i].fmpass_encode.via2, ticketformatdata.issue_info_[j].fmpass_via_station2);
                  // 経由３
                  station_code_change(format_table_.format_data_[ft_i].fmpass_encode.via3, ticketformatdata.issue_info_[j].fmpass_via_station3);
                  // 経由４
                  station_code_change(format_table_.format_data_[ft_i].fmpass_encode.via4, ticketformatdata.issue_info_[j].fmpass_via_station4);
                  // 経由５
                  station_code_change(format_table_.format_data_[ft_i].fmpass_encode.via5, ticketformatdata.issue_info_[j].fmpass_via_station5);
                  // 経由６
                  station_code_change(format_table_.format_data_[ft_i].fmpass_encode.via6, ticketformatdata.issue_info_[j].fmpass_via_station6);
                  // 経由７
                  station_code_change(format_table_.format_data_[ft_i].fmpass_encode.via7, ticketformatdata.issue_info_[j].fmpass_via_station7);
                  // 経由８
                  station_code_change(format_table_.format_data_[ft_i].fmpass_encode.via8, ticketformatdata.issue_info_[j].fmpass_via_station8);
                }// else {
//                  for(int vt_i = 0; vt_i < via8_table_.via8_table_record_num_ && vt_i < via8_table_.via8_data_.size(); ++vt_i){
//                    if(format_table_.format_data_[ft_i].fmpass_encode.via_table_no == via8_table_.via8_data_[vt_i].via_table_no){
//                      for(int vi_i = 0; vi_i < via8_table_.via8_data_[vt_i].via_info_num && vi_i < via8_table_.via8_data_[vt_i].via8_info.size(); ++vi_i){
//                        if(my_station_code_.line == via8_table_.via8_data_[vt_i].via8_info[vi_i].line 
//                          && my_station_code_.station >= via8_table_.via8_data_[vt_i].via8_info[vi_i].station_lower 
//                          && my_station_code_.station <= via8_table_.via8_data_[vt_i].via8_info[vi_i].station_upper){
//                          // 経由１
//                          station_code_change(via8_table_.via8_data_[vt_i].via8_info[vi_i].via1, ticketformatdata.issue_info_[j].fmpass_via_station1);
//                          // 経由２
//                          station_code_change(via8_table_.via8_data_[vt_i].via8_info[vi_i].via2, ticketformatdata.issue_info_[j].fmpass_via_station2);
//                          // 経由３
//                          station_code_change(via8_table_.via8_data_[vt_i].via8_info[vi_i].via3, ticketformatdata.issue_info_[j].fmpass_via_station3);
//                          // 経由４
//                          station_code_change(via8_table_.via8_data_[vt_i].via8_info[vi_i].via4, ticketformatdata.issue_info_[j].fmpass_via_station4);
//                          // 経由５
//                          station_code_change(via8_table_.via8_data_[vt_i].via8_info[vi_i].via5, ticketformatdata.issue_info_[j].fmpass_via_station5);
//                          // 経由６
//                          station_code_change(via8_table_.via8_data_[vt_i].via8_info[vi_i].via6, ticketformatdata.issue_info_[j].fmpass_via_station6);
//                          // 経由７
//                          station_code_change(via8_table_.via8_data_[vt_i].via8_info[vi_i].via7, ticketformatdata.issue_info_[j].fmpass_via_station7);
//                          // 経由８
//                          station_code_change(via8_table_.via8_data_[vt_i].via8_info[vi_i].via8, ticketformatdata.issue_info_[j].fmpass_via_station8);
//                          break;
//                        }
//                      }
//                      break;
//                    }
//                  }
//                }
                ticketformatdata.issue_info_[j].fmpass_doubleness_bit = format_table_.format_data_[ft_i].fmpass_encode.doubleness_bit;
              } else if(ticketformatdata.issue_info_[j].ticket_kind == ENCODE_FIX){
                // 大型券
                ticketformatdata.encode_num_++;
                // 券種３
                ticketformatdata.issue_info_[j].fmbigst_ticket_kind3 = format_table_.format_data_[ft_i].fmbigst_encode.ticket_kind3;
                // 往復情報
                ticketformatdata.issue_info_[j].fmbigst_roundtrip_info = format_table_.format_data_[ft_i].fmbigst_encode.roundtrip_info;
                // 復券
                if(ticketformatdata.issue_info_[j].fmbigst_roundtrip_info == 2){
                  // 着駅１→発駅１
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.start1, ticketformatdata.issue_info_[j].fmbigst_arrive_station1);
                  // 着駅２→発駅２
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.start2, ticketformatdata.issue_info_[j].fmbigst_arrive_station2);
                  // 着駅３→発駅３
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.start3, ticketformatdata.issue_info_[j].fmbigst_arrive_station3);
                  // 発駅１→着駅１
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.arrive1, ticketformatdata.issue_info_[j].fmbigst_start_station1);
                  // 発駅２→着駅２
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.arrive2, ticketformatdata.issue_info_[j].fmbigst_start_station2);
                  // 発駅３→着駅３
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.arrive3, ticketformatdata.issue_info_[j].fmbigst_start_station3);
                  // 経由テーブル
                  if(format_table_.format_data_[ft_i].fmbigst_encode.via_table_no == 0){
                    LineCode via_a[10],via_b[10];
                    via_a[0] = format_table_.format_data_[ft_i].fmbigst_encode.via1;
                    via_a[1] = format_table_.format_data_[ft_i].fmbigst_encode.via2;
                    via_a[2] = format_table_.format_data_[ft_i].fmbigst_encode.via3;
                    via_a[3] = format_table_.format_data_[ft_i].fmbigst_encode.via4;
                    via_a[4] = format_table_.format_data_[ft_i].fmbigst_encode.via5;
                    via_a[5] = format_table_.format_data_[ft_i].fmbigst_encode.via6;
                    via_a[6] = format_table_.format_data_[ft_i].fmbigst_encode.via7;
                    via_a[7] = format_table_.format_data_[ft_i].fmbigst_encode.via8;
                    via_a[8] = format_table_.format_data_[ft_i].fmbigst_encode.via9;
                    via_a[9] = format_table_.format_data_[ft_i].fmbigst_encode.via10;
                    int via_ai = sizeof(via_a)/sizeof(LineCode);
                    int via_bi = 1;
                    for(; via_ai > 0; --via_ai){
                      if(via_a[via_ai - 1].line != 0 || via_a[via_ai - 1].station != 0) {
                        via_b[via_bi - 1] = via_a[via_ai - 1];
                        ++via_bi;
                      }
                    }
                    station_code_change(via_b[0], ticketformatdata.issue_info_[j].fmbigst_via_station1);
                    station_code_change(via_b[1], ticketformatdata.issue_info_[j].fmbigst_via_station2);
                    station_code_change(via_b[2], ticketformatdata.issue_info_[j].fmbigst_via_station3);
                    station_code_change(via_b[3], ticketformatdata.issue_info_[j].fmbigst_via_station4);
                    station_code_change(via_b[4], ticketformatdata.issue_info_[j].fmbigst_via_station5);
                    station_code_change(via_b[5], ticketformatdata.issue_info_[j].fmbigst_via_station6);
                    station_code_change(via_b[6], ticketformatdata.issue_info_[j].fmbigst_via_station7);
                    station_code_change(via_b[7], ticketformatdata.issue_info_[j].fmbigst_via_station8);
                    station_code_change(via_b[8], ticketformatdata.issue_info_[j].fmbigst_via_station9);
                    station_code_change(via_b[9], ticketformatdata.issue_info_[j].fmbigst_via_station10);
                  }// else {
//                    for(int vt_i = 0; vt_i < via10_table_.via10_table_record_num_ && vt_i < via10_table_.via10_data_.size(); ++vt_i){
//                      if(format_table_.format_data_[ft_i].fmbigst_encode.via_table_no == via10_table_.via10_data_[vt_i].via_table_no){
//                        for(int vi_i = 0; vi_i < via10_table_.via10_data_[vt_i].via_info_num && vi_i < via10_table_.via10_data_[vt_i].via10_info.size(); ++vi_i){
//                          if(my_station_code_.line == via10_table_.via10_data_[vt_i].via10_info[vi_i].line 
//                            && my_station_code_.station >= via10_table_.via10_data_[vt_i].via10_info[vi_i].station_lower 
//                            && my_station_code_.station <= via10_table_.via10_data_[vt_i].via10_info[vi_i].station_upper){
//                            LineCode via_a[10],via_b[10];
//                            via_a[0] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via1;
//                            via_a[1] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via2;
//                            via_a[2] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via3;
//                            via_a[3] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via4;
//                            via_a[4] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via5;
//                            via_a[5] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via6;
//                            via_a[6] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via7;
//                            via_a[7] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via8;
//                            via_a[8] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via9;
//                            via_a[9] = via10_table_.via10_data_[vt_i].via10_info[vi_i].via10;
//                            int via_ai = sizeof(via_a)/sizeof(LineCode);
//                            int via_bi = 1;
//                            for(; via_ai > 0; --via_ai){
//                              if(via_a[via_ai - 1].line != 0 || via_a[via_ai - 1].station != 0) {
//                                via_b[via_bi - 1] = via_a[via_ai - 1];
//                                ++via_bi;
//                              }
//                            }
//                            station_code_change(via_b[0], ticketformatdata.issue_info_[j].fmbigst_via_station1);
//                            station_code_change(via_b[1], ticketformatdata.issue_info_[j].fmbigst_via_station2);
//                            station_code_change(via_b[2], ticketformatdata.issue_info_[j].fmbigst_via_station3);
//                            station_code_change(via_b[3], ticketformatdata.issue_info_[j].fmbigst_via_station4);
//                            station_code_change(via_b[4], ticketformatdata.issue_info_[j].fmbigst_via_station5);
//                            station_code_change(via_b[5], ticketformatdata.issue_info_[j].fmbigst_via_station6);
//                            station_code_change(via_b[6], ticketformatdata.issue_info_[j].fmbigst_via_station7);
//                            station_code_change(via_b[7], ticketformatdata.issue_info_[j].fmbigst_via_station8);
//                            station_code_change(via_b[8], ticketformatdata.issue_info_[j].fmbigst_via_station9);
//                            station_code_change(via_b[9], ticketformatdata.issue_info_[j].fmbigst_via_station10);
//                            break;
//                          }
//                        }
//                        break;
//                      }
//                    }
//                  }
                } else {
                  // 復券以外
                  // 発駅
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.start1, ticketformatdata.issue_info_[j].fmbigst_start_station1);
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.start2, ticketformatdata.issue_info_[j].fmbigst_start_station2);
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.start3, ticketformatdata.issue_info_[j].fmbigst_start_station3);
                  // 着駅
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.arrive1, ticketformatdata.issue_info_[j].fmbigst_arrive_station1);
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.arrive2, ticketformatdata.issue_info_[j].fmbigst_arrive_station2);
                  station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.arrive3, ticketformatdata.issue_info_[j].fmbigst_arrive_station3);
                  // 経由テーブル
                  if(format_table_.format_data_[ft_i].fmbigst_encode.via_table_no == 0){
                    // 経由１
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via1, ticketformatdata.issue_info_[j].fmbigst_via_station1);
                    // 経由２
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via2, ticketformatdata.issue_info_[j].fmbigst_via_station2);
                    // 経由３
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via3, ticketformatdata.issue_info_[j].fmbigst_via_station3);
                    // 経由４
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via4, ticketformatdata.issue_info_[j].fmbigst_via_station4);
                    // 経由５
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via5, ticketformatdata.issue_info_[j].fmbigst_via_station5);
                    // 経由６
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via6, ticketformatdata.issue_info_[j].fmbigst_via_station6);
                    // 経由７
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via7, ticketformatdata.issue_info_[j].fmbigst_via_station7);
                    // 経由８
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via8, ticketformatdata.issue_info_[j].fmbigst_via_station8);
                    // 経由９
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via9, ticketformatdata.issue_info_[j].fmbigst_via_station9);
                    // 経由１０
                    station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.via10, ticketformatdata.issue_info_[j].fmbigst_via_station10);
                  }// else {
//                    for(int vt_i = 0; vt_i < via10_table_.via10_table_record_num_ && vt_i < via10_table_.via10_data_.size(); ++vt_i){
//                      if(format_table_.format_data_[ft_i].fmbigst_encode.via_table_no == via10_table_.via10_data_[vt_i].via_table_no){
//                        for(int vi_i = 0; vi_i < via10_table_.via10_data_[vt_i].via_info_num && vi_i < via10_table_.via10_data_[vt_i].via10_info.size(); ++vi_i){
//                          if(my_station_code_.line == via10_table_.via10_data_[vt_i].via10_info[vi_i].line 
//                            && my_station_code_.station >= via10_table_.via10_data_[vt_i].via10_info[vi_i].station_lower 
//                            && my_station_code_.station <= via10_table_.via10_data_[vt_i].via10_info[vi_i].station_upper){
//                            // 経由１
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via1, ticketformatdata.issue_info_[j].fmbigst_via_station1);
//                            // 経由２
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via2, ticketformatdata.issue_info_[j].fmbigst_via_station2);
//                            // 経由３
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via3, ticketformatdata.issue_info_[j].fmbigst_via_station3);
//                            // 経由４
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via4, ticketformatdata.issue_info_[j].fmbigst_via_station4);
//                            // 経由５
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via5, ticketformatdata.issue_info_[j].fmbigst_via_station5);
//                            // 経由６
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via6, ticketformatdata.issue_info_[j].fmbigst_via_station6);
//                            // 経由７
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via7, ticketformatdata.issue_info_[j].fmbigst_via_station7);
//                            // 経由８
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via8, ticketformatdata.issue_info_[j].fmbigst_via_station8);
//                            // 経由９
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via9, ticketformatdata.issue_info_[j].fmbigst_via_station9);
//                            // 経由１０
//                            station_code_change(via10_table_.via10_data_[vt_i].via10_info[vi_i].via10, ticketformatdata.issue_info_[j].fmbigst_via_station10);
//                            break;
//                          }
//                        }
//                        break;
//                      }
//                    }
//                  }
                }
                // フリー区間コード
                station_code_change(format_table_.format_data_[ft_i].fmbigst_encode.free_area_code, ticketformatdata.issue_info_[j].fmbigst_free_area_code);
                // 券種４詳細
                ticketformatdata.issue_info_[j].fmbigst_ticket_kind4_detail = format_table_.format_data_[ft_i].fmbigst_encode.ticket_kind4_detail;
                // 時差・土休
                ticketformatdata.issue_info_[j].fmbigst_ticket_type = format_table_.format_data_[ft_i].fmbigst_encode.ticket_type;
                // 商品コード
                ticketformatdata.issue_info_[j].fmbigst_item_code = format_table_.format_data_[ft_i].fmbigst_encode.item_code;
                // 割引コード
                ticketformatdata.issue_info_[j].fmbigst_discount_code = format_table_.format_data_[ft_i].fmbigst_encode.discount_code;
              }
              break;
            }
          }
        }
      }
      ticketformat_data_.push_back(ticketformatdata);
    }
    ret = true;
  } catch(...){}
  return ret;
}


inline EachExtraTicketData::EachExtraTicketData():generation_(0), ticketnum_(0) {}

inline EachExtraTicketData::~EachExtraTicketData(){}

inline bool EachExtraTicketData::internalize(const char*& data, int generation, int ticketnum)
{
  bool ret = false;
  try {
    generation_ = generation;
    ticketnum_ = ticketnum;
    if(master_.internalize(data, generation_, ticketnum_) && format_.internalize(data, generation_)) {
      ret = true;
    }
  } catch(...){}
  return ret;
}


inline ExtraTicketData::ExtraTicketData(){}

inline ExtraTicketData::~ExtraTicketData(){}

inline bool ExtraTicketData::internalize(const char* data, long size, int& error_detail)
{
  bool ret = false;
  try {
    const char* pos = data;
    error_detail = ERROR_OTHER;
    // long data_size = 0;
//    if(prefix_.internalize(pos)) // 通信部ヘッダ 名臨高がない
    {
//      size = (sizeof PREFIX) + prefix_.size_;
      // サイズチェック 削除した
      // if(data_size == size && comprefix_.internalize(pos)) 
      if(comprefix_.internalize(pos)) 
      {
        // 設置駅コード一致性チェック
        unsigned long station_setting_code = 0;
        SettingData sd;
        station_setting_code = sd.lget_data(SET_STATION, HOST_CODE);
        if(comprefix_.station_setting_code_ == station_setting_code) {
          bool ret2 = true;
          // 新データ
//          const char* pos_new = data + sizeof PREFIX + comprefix_.newdata_offset_;
          const char* pos_new = data + comprefix_.newdata_offset_;
          if(comprefix_.comdata_[0].master_record_count_ != 0) {
            ret2 &= eachdata_[0].internalize(pos_new, 1, comprefix_.comdata_[0].master_record_count_);
          }
          // 現行データ
//          const char* pos_cur = data + sizeof PREFIX + comprefix_.curdata_offset_;
          const char* pos_cur = data + comprefix_.curdata_offset_;
          if(comprefix_.comdata_[1].master_record_count_ != 0) {
            ret2 &= eachdata_[1].internalize(pos_cur, 2, comprefix_.comdata_[1].master_record_count_);
          }
          // if(ret2 && pos - data == data_size){ ret = true; }
          if(ret2){ ret = true; error_detail = ERROR_OK; }
        } else {
          error_detail = ERROR_STATION;
        }
      }
    }
  } catch(...){}
  return ret;
}


inline ETFmtBmpMaker::ETFmtBmpMaker(){}

inline ETFmtBmpMaker::~ETFmtBmpMaker(){}

/*---------------------------------*/
/* ファイルヘッダ BITMAPFILEHEADER */
/*---------------------------------*/
inline unsigned long ETFmtBmpMaker::fileheader(FILE* fp, unsigned long size, unsigned long offset)
{
  unsigned long count, filesize;
  short reserved = 0;
  char s[2];
  
  rewind(fp);
  
  // 識別文字 BM
  s[0] = 'B';
  s[1] = 'M';
  fwrite(s, sizeof(char), sizeof s, fp);
  // ファイルサイズ bfSize 
  filesize = size + offset;
  fwrite(&filesize, sizeof(unsigned long), 1, fp);
  // 予約エリア bfReserved1 
  fwrite(&reserved, sizeof(short), 1, fp);
  // 予約エリア bfReserved2 
  fwrite(&reserved, sizeof(short), 1, fp);
  // データ部までのオフセット bfOffBits 
  fwrite(&offset, sizeof(unsigned long), 1, fp);

  // ファイルヘッダサイズ 14 Byte 
  if ((count = ftell(fp)) != 14) {;}

  return count;
}

/*-----------------------------*/
/* 情報ヘッダ BITMAPINFOHEADER */
/*-----------------------------*/
inline unsigned long ETFmtBmpMaker::bitmapheader(FILE * fp,
                                          unsigned long width, unsigned long height, unsigned long size)
{
  unsigned long count, var_long;
  unsigned long widhei;
  short var_short;

  fseek(fp, 14, SEEK_SET);

  // 情報ヘッダのサイズ biSize (Windows BMP は 40) 
  var_long = 40;
  fwrite(&var_long, sizeof(unsigned long), 1, fp);
  // 画像の幅 biWidth 
  widhei = width;
  fwrite(&widhei, sizeof(unsigned long), 1, fp);
  // 画像の高さ biHeight 
  // (正数ならば左下から右上, マイナスならば左上から右下) 
  widhei = height;
  fwrite(&widhei, sizeof(unsigned long), 1, fp);
  // プレーン数 biPlanes (必ず 1) 
  var_short = 1;
  fwrite(&var_short, sizeof(short), 1, fp);
  // 1ピクセルのデータ数 biBitCount (1, 4, 8, 24, 32) 
  var_short = (short)BITCOUNT;
  fwrite(&var_short, sizeof(short), 1, fp);

  // 圧縮 biCompression (無圧縮ならば 0) 
  var_long = 0;
  fwrite(&var_long, sizeof(unsigned long), 1, fp);
  // 画像のサイズ biSizeImage 
  var_long = size;
  fwrite(&var_long, sizeof(unsigned long), 1, fp);
  // 横方向解像度 pixel/m biXPelPerMeter 
  // (96dpi, 1inch = 0.0254m のとき 96/0.0254 = 3780) 
  var_long = 3780;
  fwrite(&var_long, sizeof(unsigned long), 1, fp);
  // 縦方向解像度 pixel/m biYPelPerMeter 
  // (96dpi, 1inch = 0.0254m のとき 96/0.0254 = 3780) 
  var_long = 3780;
  fwrite(&var_long, sizeof(unsigned long), 1, fp);
  // パレット数 biClrUsed 
  var_long = 1 << BITCOUNT;
  fwrite(&var_long, sizeof(unsigned long), 1, fp);
  // パレット中の重要な色  biClrImportant 
  var_long = 0;
  fwrite(&var_long, sizeof(unsigned long), 1, fp);

  // ファイルヘッダ(14 Byte) + 情報ヘッダサイズ(40 Byte) 
  if ((count = ftell(fp)) != 54) {;}

  return count;
}

/*------------------------*/
/* パレットデータ RGBQUAD */
/*------------------------*/
inline unsigned long ETFmtBmpMaker::rgbquad(FILE * fp)
{
  unsigned long i, color, count;

  fseek(fp, 54, SEEK_SET);

  color = 1 << BITCOUNT;

  for (i = 0; i < color; i++) {
    unsigned char red, green, blue, reserved = 0;

    if (i==0)
    {
      red = green = blue =0xff;
    } else {
      red = green = blue =0x00;
    }

    fputc(blue, fp);
    fputc(green, fp);
    fputc(red, fp);
    fputc(reserved, fp);
  }

  if ((count = ftell(fp)) != 54 + 4 * color) {;}

  return count;
}

/*------------*/
/* 画像データ */
/*------------*/
inline unsigned long ETFmtBmpMaker::imagedata(FILE * fp, unsigned long datalng,
                                       const unsigned char* data, unsigned long offset)
{

  fseek(fp, offset, SEEK_SET);
  fwrite(data, sizeof(unsigned char), datalng, fp);

  return ftell(fp);
}

/*-----------------*/
/* BITMAP 書き込み */
/*-----------------*/
inline bool ETFmtBmpMaker::MakeFontBmp(const char* filename, bool isYOKO,
                                unsigned short width, unsigned short height, const unsigned char* data)
{
  bool ret = false;
  try {
    unsigned long sy = 0;
    unsigned long sx = 0;
    unsigned long lineByte = 0;
    unsigned long xdots = 0;
    unsigned long ydots = 0;
    unsigned long size = 0;
    unsigned long offset = 0;
    unsigned char *pdata = NULL;
    FILE* fp2;

    // エリアサイズ数
    xdots = width;

    ydots = height;

    // 横　isYOKO = true;
    if(ydots % 8 != 0) {
      sy = ((ydots / 8 ) + 1);
    } else {
      sy = ydots / 8;
    }
    // 縦　isYOKO = false;
    if(xdots % 8 != 0) {
      sx = ((xdots / 8 ) + 1);
    } else {
      sx = xdots / 8;
    }

    unsigned long fsts = GetFileAttributes(filename);
    if((fsts & FILE_ATTRIBUTE_READONLY) != 0){
      fsts &= ~FILE_ATTRIBUTE_READONLY; // 読取専用属性を削除
      SetFileAttributes(filename,fsts);
    }

    if ((fp2 = fopen(filename, "wb")) == NULL) {
      return false;
    }

    // 画像サイズ (1ラインは4Byte(long)境界にあわせる) 
    if (isYOKO) {
      lineByte = sy;
      if (sy % 4 != 0) {
        lineByte = ((sy / 4 + 1) * 4);
      }
      size = lineByte * xdots;
    } else {
      lineByte = sx;
      if (sx % 4 != 0) {
        lineByte = ((sx / 4 + 1) * 4);
      }
      size = lineByte * ydots;
    }

    pdata = 0;
    pdata  = (unsigned char*)malloc(size);
    ZeroMemory(pdata, size);

    font2bit(width, height, data, pdata, lineByte, isYOKO);

    // 画像データまでのオフセット(ヘッダサイズ) 
    offset = 14 + 40 + 4 * (1 << BITCOUNT);

    // ファイルヘッダ 
    fileheader(fp2, size, offset);
    // 情報ヘッダ 
    if (isYOKO) {
      bitmapheader(fp2, ydots, xdots, size);
    } else {
      bitmapheader(fp2, xdots, ydots, size);
    }
    // パレットデータ 
    rgbquad(fp2);

    // 画像データ 
    imagedata(fp2, size, pdata, offset);

    fclose(fp2);
    free(pdata);
    ret = true;
  } catch(...){}
  return ret;
}

inline void ETFmtBmpMaker::font2bit(unsigned short width, unsigned short height,
                             const unsigned char* pFont, unsigned char*& pOut, unsigned long LineByte, bool isYOKO)
{
  unsigned long ypix = height;
  unsigned long xpix = width;
  unsigned long byteofs = 0;
  unsigned long bitofs = 0;
  unsigned long colofs = 0;
  unsigned char bitmask = 0;
  unsigned long allofs = 0;
  unsigned long bitcount = 0;
  for (unsigned long x= 0 ; x < xpix ; ++x)
  {
    byteofs = 0;
    for (unsigned long y = 0; y < ypix; ++y)
    {
      bitofs = ((bitcount + y) % 8);
      bitmask = 1 << bitofs;
      allofs = colofs + byteofs;
      if((pFont[allofs] & bitmask) !=0) 
      { // ドットあり
        if (isYOKO) {
          setdot_yoko(x, y, pOut, LineByte);
        } else {
          //setdot_tate_h(x, y, pOut, LineByte, ypix);
          setdot_tate_w(x, y, pOut, LineByte, xpix);
        }
      }
      if((y + 1 + bitcount) % 8 == 0 ) {
        ++byteofs;
      }
      if((y + 1) >= ypix) // 今回で１列終了
      {
        bitcount = (bitofs + 1) % 8;
        break;
      }
    }
    colofs += byteofs;
  }
}

inline void ETFmtBmpMaker::setdot_yoko(unsigned long x, unsigned long y, unsigned char*& pOut, unsigned long LineByte)
{
  //横
  unsigned long lineofs = x * LineByte;
  unsigned long byteofs = y / 8;
  unsigned char bitmask = 0x80 >> (y % 8);
  pOut[lineofs + byteofs] |= bitmask;
  
}

inline void ETFmtBmpMaker::setdot_tate_h(unsigned long x, unsigned long y, unsigned char*& pOut, unsigned long LineByte, unsigned long height)
{
  //縦
  //画面方向↑→↑
  unsigned long lineofs = (height - 1 - y) * LineByte;
  unsigned long byteofs = x / 8;
  unsigned char bitmask = 0x80 >> (x % 8);
  pOut[lineofs + byteofs] |= bitmask;
  
}

inline void ETFmtBmpMaker::setdot_tate_w(unsigned long x, unsigned long y, unsigned char*& pOut, unsigned long LineByte, unsigned long width)
{
  //縦
  //画面方向↑→↓
  unsigned long lineofs = y * LineByte;
  unsigned long byteofs = (width - 1 - x) / 8;
  unsigned char bitmask = 0x80 >> ((width - 1 - x) % 8);
  pOut[lineofs + byteofs] |= bitmask;
  
}


inline TicketFaceFileEdit::TicketFaceFileEdit(std::string filename)
{
  file_name_ = filename;
}
inline TicketFaceFileEdit::~TicketFaceFileEdit(){}

// ベース券面ファイル（Ａ～Ｒ）を読み込む。
inline bool TicketFaceFileEdit::open(std::string basefilename)
{
  bool ret = false;
  txt_buff_.clear();
  locale::global(locale(""));
  ifstream fin(basefilename.c_str());
  locale::global(locale("C"));
  if(fin.is_open()) {
    std::string txtbuf = "";
    while (fin.good()) {
      getline(fin, txtbuf);
      txt_buff_.push_back(txtbuf);
    }
    fin.close();
    ret = true;
  }
  return ret;
}

inline bool TicketFaceFileEdit::replaceFixedBmpField(int x,int y,std::string stReplaceTo)
{
  bool bZone = false;
  bool bHIT = false;
  ExtraTicketUtils etu;
  vector<std::string>::iterator itor = txt_buff_.begin();
  for (int i = 0;i < txt_buff_.size(); ++i,++itor)
  {
    std::string swk = txt_buff_[i];
    swk = etu.string_trim(swk);
    if(swk[0] == '/' && swk[1] == '/')
    {
      continue;
    }

    if(swk == "[fixed]")
    {
      bZone = true;
      continue;
    }
    if(bZone && swk[0] == '[')
    {
      bZone = false;
      break;
    }
    
    if(bZone) {
      // 空行の場合は
      if(etu.string_trim(swk) == ""){
        std::ostringstream oss;
        oss << x << "," << y << "," << stReplaceTo;
        txt_buff_.insert(itor, oss.str());
        bHIT = true;
        break;
      }
      // 空行ではない場合は
      std::string stx,sty;
      int mark = 0;
      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      stx = swk.substr(0, mark);
      swk = swk.substr(mark + 1);
      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      sty = swk.substr(0, mark);

      int sx = atoi(stx.c_str());
      int sy = atoi(sty.c_str());
      if( sx == x && sy == y)
      {
        bHIT = true;
        std::ostringstream oss;
        oss << stx << "," << sty << "," << stReplaceTo;
        txt_buff_[i] = oss.str();
        break;
      }
      // 見つからなかった場合は追加
      if( i < txt_buff_.size() - 1)// 最終行以外
      {
        std::string swk2 = txt_buff_[i + 1]; //次の行を先読み
        if(etu.string_trim(swk2)== "")
        { // 空行　該当フィールドの最終行
          std::ostringstream oss;
          oss << x << "," << y << "," << stReplaceTo;
          txt_buff_.insert(itor+1, oss.str());
          bHIT = true;
          break;
        }
      }
    }
  }
  return bHIT;
}

//数値可変項目を削除　（特殊エリアと発売金額が重複する場合、発売金額エリアを削除）
inline bool TicketFaceFileEdit::removeNumFlexibleField(int x,int y,std::string stKeyword)
{
  bool bZone = false;
  bool bHIT = false;
  ExtraTicketUtils etu;
  for (int i = 0;i < txt_buff_.size(); ++i)
  {
    std::string swk = txt_buff_[i];
    swk = etu.string_trim(swk);
    if(swk[0] == '/' && swk[1] == '/')
    {
      continue;
    }
    if(swk == "[num-flexible]")
    {
      bZone = true;
      continue;
    }
    if(bZone && swk[0] == '[')
    {
      bZone = false;
      break;
    }
    if(bZone) {
      std::string stcnt,stx,sty;
      int mark = 0;
      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      stcnt = swk.substr(0, mark);
      swk = swk.substr(mark + 1);
      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      stx = swk.substr(0, mark);
      swk = swk.substr(mark + 1);
      mark = swk.find_first_of(',');
      if(mark <0) {
        continue;
      }
      sty = swk.substr(0, mark);
      swk = swk.substr(mark + 1);
      int sx = atoi(stx.c_str());
      int sy = atoi(sty.c_str());
      if( sx == x && sy == y && (swk.find_first_of(stKeyword) != string::npos) )
      { // 位置が一致 & キーワードあり
        bHIT = true;
        std::ostringstream oss;
        oss << "//" << stKeyword;
        txt_buff_[i] = oss.str();
        break;
      }
    }
  }
  return bHIT;
}

//パターン可変項目を削除　（特殊エリアと重複する場合削除）
inline bool TicketFaceFileEdit::removePatFlexibleField(int x,int y)
{
  bool bZone = false;
  bool bHIT = false;
  ExtraTicketUtils etu;
  for (int i = 0;i < txt_buff_.size(); ++i)
  {
    std::string swk = txt_buff_[i];
    swk = etu.string_trim(swk);
    if(swk[0] == '/' && swk[1] == '/')
    {
      continue;
    }

    if(swk == "[pat-flexible]")
    {
      bZone = true;
      continue;
    }
    if(bZone && swk[0] == '[')
    {
      bZone = false;
      break;
    }
    if(bZone) {
      std::string stx,sty;
      int mark = 0;
      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      stx = swk.substr(0, mark);
      swk = swk.substr(mark + 1);
      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      sty = swk.substr(0, mark);

      int sx = atoi(stx.c_str());
      int sy = atoi(sty.c_str());
      if(sx == x && sy == y)
      { // 位置が一致 & キーワードあり
        bHIT = true;
        std::ostringstream oss;
        oss << "//" << txt_buff_[i];
        txt_buff_[i] = oss.str();
        break;
      }
    }
  }
  return bHIT;
}

inline bool TicketFaceFileEdit::replaceFixedTxtField(vector<std::string> stReplaceToAry)
{
  bool bZone = false;
  bool bHIT = false;
  int nReplaceedCnt=0;
  ExtraTicketUtils etu;
  if(stReplaceToAry.size() ==0)
  {
    return false;
  }
  for (int i = 0;i < txt_buff_.size(); ++i)
  {
    std::string swk = txt_buff_[i];
    swk = etu.string_trim(swk);
    if(swk[0] == '/' && swk[1] == '/')
    {
      continue;
    }

    if(swk == "[vec-fixed]")
    {
      bZone = true;
      continue;
    }
    if(bZone && swk[0] == '[')
    {
      bZone = false;
      break;
    }
    if(bZone) {

      std::string stx,sty,stw,sth;
      int mark = 0;
      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      stx = swk.substr(0, mark);
      swk = swk.substr(mark + 1);

      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      sty = swk.substr(0, mark);
      swk = swk.substr(mark + 1);

      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      stw = swk.substr(0, mark);
      swk = swk.substr(mark + 1);

      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      sth = swk.substr(0, mark);
      swk = swk.substr(mark + 1);

      std::string dmytxt;
      mark = swk.find_first_of(',');
      if(mark < 0) {
        continue;
      }
      dmytxt = swk.substr(0, mark);
      swk = swk.substr(mark + 1);
      if(etu.string_trim(dmytxt) == string("\"　　　　　　　　　　　　　　　　　\"")) // 全角１７文字
      {
        std::string params[8];
        for(int p = 0 ;p < 8; ++p)
        {
          mark = swk.find_first_of(',');
          params[p] = swk.substr(0, mark);
          swk = swk.substr(mark + 1);
        }

        std::ostringstream oss;
        oss << stx << ","
            << sty << ","
            << stw << ","
            << sth << ","
            << " \"" << stReplaceToAry[nReplaceedCnt] << "\","
            << params[0] << ","
            << params[1] << ","
            << params[2] << ","
            << params[3] << ","
            << params[4] << ","
            << params[5] << ","
            << params[6] << ","
            << params[7];
        txt_buff_[i] = oss.str();
        nReplaceedCnt++;
        if(stReplaceToAry.size() <= nReplaceedCnt) {
          // 全て置換した
          bHIT = true;
          break;
        }
        // 引き続き検索
      }
    }
  }
  return bHIT;
}

inline bool TicketFaceFileEdit::save()
{
  bool ret = false;

  unsigned long fsts = GetFileAttributes(file_name_.c_str());
  if((fsts & FILE_ATTRIBUTE_READONLY) != 0){
    fsts &= ~FILE_ATTRIBUTE_READONLY; // 読取専用属性を削除
    SetFileAttributes(file_name_.c_str(),fsts);
  }

  ofstream fout(file_name_.c_str(),ios::out | ios::trunc);
  if(fout.is_open()) {
    for(int i = 0; i < txt_buff_.size(); ++i) {
      fout << txt_buff_[i] << "\n";
    }
    fout.close();
    ret = true;
  }
  return ret;
}

inline ExtraTicketUtils::ExtraTicketUtils(){}

inline ExtraTicketUtils::~ExtraTicketUtils(){}

inline std::string ExtraTicketUtils::string_trim(const std::string& str, const char* delim)
{
  const int p1(str.find_first_not_of(delim));
  if(p1 == string::npos){
    return string();
  }
  const int p2(str.find_last_not_of(delim));
  return str.substr(p1, p2 - p1 + 1);
}

// end of file