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

static const int MASTER_NUM     = 2;     //�}�X�^��
static const int ISSUE_NUM      = 4;     //����
static const int TICKET_NUM     = 100;   //���퐔
static const int ACCOUNT_NUM    = 50;    //������
static const int BITCOUNT       = 1;     //�r�b�g�}�b�v���Ɣ�
static const int Y_MAX          = 480;   //���ʃt�@�C���ɉ��ő�T�C�Y

static const char* const SET_STATION = "SET_STATION";                   // �ݒu�w�R�[�h
static const char* const AREA_CODE   = "AREA_CODE";                     //   �n��R�[�h
static const char* const USER_CODE   = "USER_CODE";                     //   ���[�U�[�R�[�h
static const char* const CODE        = "CODE";                          //   �ݒu�w����w��
static const char* const HOST_CODE   = "HOST_CODE";                     //   �z�X�g�p�ݒu�w�R�[�h

static const char* const EXTRA_TICKET_FACE_NAME = "extra_ticket";
static const char* const UNDER_BAR = "_";
static const char* const TICKET_FACE_EXT = ".tf";
static const char* const BITMAP_EXT = ".bmp";
static const char* const EXTRA_TICKET_FACE_PATH = "..\\ticketface\\tickettable\\";
static const char* const EXTRA_TICKET_BMP_PATH = "..\\ticketface\\bitmaps\\extra_ticket\\";

enum EncodeType {
	ENCODE_EDO          = 1,              // �G�h��
	ENCODE_PASS         = 2,              // �����               ���g�p��
	ENCODE_FIX          = 3,              // �����Œ�             ���g�p��
	ENCODE_FIX_SECTION  = 4,              // �����Œ�{�搔��
	ENCODE_SECTION      = 5,              // �搔��
    ENCODE_EDO_NOENCODE = 6,              // �G�h�^����
    ENCODE_BIG_NOENCODE = 7,              // ��^����             ���g�p��
};

enum ErrorDetail {
  ERROR_OK,
  ERROR_STATION,
  ERROR_OTHER,
};

//����w���p
struct LineCode {
	unsigned char line;
	unsigned char station;
	LineCode() : line(0), station(0) {}
};


// �w�b�_ 
//class Prefix {
//public:
//	unsigned char set_version_;     //�ݒ�o�[�W�����i�}�X�^�o�[�W�����ʒm�̐ݒ�o�[�W�����j
//	unsigned char master_version_;  //�}�X�^�o�[�W�����i�}�X�^�f�[�^�v���̃o�[�W�����j
//	unsigned long size_;            //�f�[�^�T�C�Y
//	unsigned long sum_;             //�T���l
//
//	std::string st_set_version_;       //�ݒ�o�[�W�����i������j
//	std::string st_master_version_;    //�}�X�^�o�[�W�����i������j
//
//	Prefix();
//	virtual ~Prefix();
//
//	bool internalize(const char*& data);
//};


// ��挔���ʃw�b�_�i�P���㕪�j
class EachComData {
public:

	Timestamp valid_date_;                                // �K�p��
	unsigned short data_version_;                         // ��挔�f�[�^�o�[�W����
	Timestamp data_create_time_;                          // ��挔�f�[�^�쐬���t�E����
	unsigned short master_version_;                       // ��挔�}�X�^�o�[�W����
	Timestamp master_create_time_;                        // ��挔�}�X�^�쐬���t�E����
	unsigned short format_version_;                       // ��挔�t�H�[�}�b�g�o�[�W����
	Timestamp format_create_time_;                        // ��挔�t�H�[�}�b�g�쐬���t�E����
	unsigned short master_record_count_;                  // ��挔�}�X�^���R�[�h����
//	unsigned short right_display_;                        // ��挔�\���ݒ�i�E�j
//	unsigned short left_display_;                         // ��挔�\���ݒ�i���j
//	unsigned short right_display_sp_;                     // ��挔�\���ݒ�i�E�j�����g�p�w�p
//	unsigned short left_display_sp_;                      // ��挔�\���ݒ�i���j�����g�p�w�p
	unsigned short button_display_order_[ACCOUNT_NUM];    // �����{�^���\������
	unsigned short button_display_order_sp_[ACCOUNT_NUM]; // �����{�^���\������  �����g�p�w�p
	unsigned short account_number_;                       // ������
//	unsigned short account_number_sp_;                    // ������              �����g�p�w�p
	std::string st_data_version_;                         // ��挔�f�[�^�o�[�W�����i������j

	EachComData();
	virtual ~EachComData();

	bool internalize(const char*& data);
};


// ��挔���ʃw�b�_
class ComPrefix {
public:

	unsigned long station_setting_code_;       // �ݒu�w�R�[�h
	Timestamp data_delivery_time_;             // ��挔�f�[�^(�z�M�f�[�^) �쐬���t�E����
	unsigned long newdata_offset_;             // �V�f�[�^�I�t�Z�b�g
	unsigned long curdata_offset_;             // ���s�f�[�^�I�t�Z�b�g
	EachComData comdata_[MASTER_NUM];          // ���ʃf�[�^�P�A�Q

	ComPrefix();
	virtual ~ComPrefix();

	bool internalize(const char*& data);
};

// ��挔������
class TicketData {
public:

	unsigned short extra_ticket_no_; // ��挔No
	unsigned short issue_data_kind_no_; // �����f�[�^���No
	unsigned short issue_data_no_; // �����f�[�^No
	unsigned short refund_data_kind_no_; // ���߃f�[�^���No
	unsigned short refund_data_no_; // ���߃f�[�^No
//	unsigned long item_code_; // ���i�R�[�h
//	unsigned short start_station_code_; // ���w�R�[�h
//	unsigned short start_company_code_; // ���ЃR�[�h
//	unsigned short junction_station_code1_; // �ڑ��w�P�R�[�h
//	unsigned short junction_company_code1_; // �ڑ��ЂP�R�[�h
//	unsigned short junction_station_code2_; // �ڑ��w�Q�R�[�h
//	unsigned short junction_company_code2_; // �ڑ��ЂQ�R�[�h
//	unsigned short junction_station_code3_; // �ڑ��w�R�R�[�h
//	unsigned short junction_company_code3_; // �ڑ��ЂR�R�[�h
//	unsigned short arrive_station_code_; // ���w�R�[�h
//	unsigned short arrive_company_code_; // ���ЃR�[�h
	unsigned short issue_amount_adult_; // �������z�i��l�j
	unsigned short issue_amount_child_; // �������z�i�����j(���Ս��F0xFFFF�Z�b�g�̂Ƃ������̂ݔ����֎~�Ƃ���)
	unsigned short issue_amount_discount_adult_; // �������z�i��l�E��Q�����j
	unsigned short issue_amount_discount_child_; // �������z�i�����E��Q�����j
	unsigned short division_amount_adult_; // �搔���z�i��l�j
	unsigned short division_amount_child_; // �搔���z�i�����j
	unsigned short other_company_amount_adult_; // ���Ў敪���z�i��l�j
	unsigned short other_company_amount_child_; // ���Ў敪���z�i�����j
	unsigned short other_company_amount_dis_adult_; // ���Ў敪���z�i��l�j
	unsigned short other_company_amount_dis_child_; // ���Ў敪���z�i�����j
	unsigned short ticketface1_fare_adult_; // �P���ڕ\���^���i��l�j
	unsigned short ticketface1_fare_child_; // �P���ڕ\���^���i�����j
	unsigned short ticketface2_fare_adult_; // �Q���ڕ\���^���i��l�j
	unsigned short ticketface2_fare_child_; // �Q���ڕ\���^���i�����j
	unsigned short ticketface3_fare_adult_; // �R���ڕ\���^���i��l�j
	unsigned short ticketface3_fare_child_; // �R���ڕ\���^���i�����j
	unsigned short ticketface4_fare_adult_; // �S���ڕ\���^���i��l�j
	unsigned short ticketface4_fare_child_; // �S���ڕ\���^���i�����j
	unsigned short issue_num_; // ���s����
	Timestamp issue_start_date_; // �����J�n�N����
	Timestamp issue_end_date_; // �����I���N����
	bool is_indefinite_; // �������ł��邩�ǂ��� �����I���N�����u99999999�v�������Ƃ���
	Timestamp pre_issue_start_date_; // �O���J�n�N����
	bool is_pre_issue_; // �O�������邩�ǂ��� �O���J�n�N�����u99999999�v�O���Ȃ��Ƃ���
//	unsigned short v_valid_date_count_; // ���@�\�����@�p�L������
	unsigned short cm_valid_date_count_; // �����p�L������
	Timestamp event_start_date_; // �C�x���g�Œ�L���J�n��
	Timestamp event_end_date_; // �C�x���g�Œ�L���I����
//	bool v_issue_permit_; // ���@�\�����@�����ۃt���O�@true ��, false ��
//	bool v_refund_permit_; // ���@�\�����@���߉ۃt���O�@true ��, false ��
	bool cm_issue_permit_; // ���������ۃt���O�@true ��, false ��
	bool cm_refund_permit_; // �������߉ۃt���O�@true ��, false ��
	bool is_holiday_; // �y�x���t���O�@true �y�x��, false �ʏ��
	unsigned short limit_weekday_set_start_hour_; // ��挔�����ݒ�i�����j�J�n��
	unsigned short limit_weekday_set_start_minute_; // ��挔�����ݒ�i�����j�J�n��
	unsigned short limit_weekday_set_end_hour_; // ��挔�����ݒ�i�����j�I����
	unsigned short limit_weekday_set_end_minute_; // ��挔�����ݒ�i�����j�I����
	unsigned short limit_holiday_set_start_hour_; // ��挔�����ݒ�i�x���j�J�n��
	unsigned short limit_holiday_set_start_minute_; // ��挔�����ݒ�i�x���j�J�n��
	unsigned short limit_holiday_set_end_hour_; // ��挔�����ݒ�i�x���j�I����
	unsigned short limit_holiday_set_end_minute_; // ��挔�����ݒ�i�x���j�I����
	bool is_fare_change_; // �^���}�[�N�@true ��, false ��
	unsigned short button_index_; // �����{�^���\������

	TicketData();
	virtual ~TicketData();

	bool internalize(const char*& data);
};


// ��挔�}�X�^
class ExtraTicketMasterData {
public:

	TicketData ticket_data_[TICKET_NUM];        // �P�O�O����

	// ����i�P�F�V�A�Q�F���s�j
	int generation_;
	// �}�X�^��
    int ticketnum_;

	ExtraTicketMasterData();
	virtual ~ExtraTicketMasterData();

  bool internalize(const char*& data, int generation, int ticketnum);
};


// ��挔�O���[�v�e�[�u��
class ExtraTicketGroupTable {
public:

	unsigned short group_table_record_num_; // �O���[�v�e�[�u���o�^���R�[�h��

	struct GroupData {       // �O���[�v�f�[�^
		unsigned short no;     //   ��挔No
		std::string name;      //   ��挔����
//		unsigned short attribute; //  ����
		unsigned short ticket1_no; // �ꖇ�ڃt�H�[�}�b�gNo
		unsigned short ticket2_no; // �񖇖ڃt�H�[�}�b�gNo
		unsigned short ticket3_no; // �O���ڃt�H�[�}�b�gNo
		unsigned short ticket4_no; // �l���ڃt�H�[�}�b�gNo
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


// ��挔�t�H�[�}�b�g�e�[�u��
class ExtraTicketFormatTable {
public:

	unsigned short format_table_record_num_; // �t�H�[�}�b�g�e�[�u���o�^���R�[�h��

	struct FMBigstEncode {       // FM��^��
		unsigned short ticket_kind3;      // ����R�@�P�T�Œ�
		LineCode start1;  // ���w�P
		LineCode start2;  // ���w�Q
		LineCode start3;  // ���w�R
		LineCode arrive1; // ���w�P
		LineCode arrive2; // ���w�Q
		LineCode arrive3; // ���w�R
		unsigned short via_table_no;      // �o�R�e�[�u��No
		LineCode via1;                     //�o�R1����w��
		LineCode via2;                     //�o�R2����w��
		LineCode via3;                     //�o�R3����w��
		LineCode via4;                     //�o�R4����w��
		LineCode via5;                     //�o�R5����w��
		LineCode via6;                     //�o�R6����w��
		LineCode via7;                     //�o�R7����w��
		LineCode via8;                     //�o�R8����w��
		LineCode via9;                     //�o�R9����w��
		LineCode via10;                    //�o�R10����w��
		LineCode free_area_code;           //�t���[��ԃR�[�h
		unsigned short ticket_kind4_detail; // ����S�ڍ�
		unsigned char roundtrip_info;           //������� 0:�w��Ȃ��A1:�����A2:����
		unsigned char ticket_type; // 0:�w��Ȃ������r�b�g�I�t�A1:�����A2:�y�x�A�ȊO:�����r�b�g�I�t
		unsigned long item_code; // ���i�R�[�h
		unsigned char discount_code;       // �����R�[�h�@�O�`�P�T
		FMBigstEncode() : ticket_kind3(0), via_table_no(0), ticket_kind4_detail(0),
			roundtrip_info(0), ticket_type(0), item_code(0), discount_code(0) {}
	};

	struct FMPassEncode {       // FM�����
		unsigned short ticket_kind3;      // ����R�@�X�Œ�
		LineCode start;  // ���w
		LineCode arrive; // ���w
		unsigned short via_table_no;      // �o�R�e�[�u��No
		LineCode via1;                     //�o�R1����w��
		LineCode via2;                     //�o�R2����w��
		LineCode via3;                     //�o�R3����w��
		LineCode via4;                     //�o�R4����w��
		LineCode via5;                     //�o�R5����w��
		LineCode via6;                     //�o�R6����w��
		LineCode via7;                     //�o�R7����w��
		LineCode via8;                     //�o�R8����w��
		bool doubleness_bit;                // ��d���@true:����(�W�g���b�N)�Afalse:�Ȃ�(�S�g���b�N)
		FMPassEncode() : ticket_kind3(0), via_table_no(0), doubleness_bit(false) {}
	};

	struct FormatData {       // �O���[�v�f�[�^
		unsigned short no;     //   �t�H�[�}�b�gNo
		std::string name;      //   ��挔����
		unsigned short ticket_kind; //  ����
//		unsigned short attribute; //  ����
		bool is_amount_print;     // ���z�󎚗L���@true �L, false ��
		LineCode start_station; // ���w
		bool is_numbering_print;  // �i���o�����O�󎚗L���@true �L, false ��
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


//// ��挔�o�R�p�^�[���e�[�u���o�R�W������p
//class ExtraTicketVia8Table {
//public:
//
//	unsigned short via8_table_record_num_; // �o�R�W�e�[�u���o�^���R�[�h��
//
//	struct Via8Info {       // �o�R���
//		unsigned short line;      // ����
//		unsigned short station_lower;      // �w������
//		unsigned short station_upper;      // �w�����
//		unsigned short via_num;      // �o�R�w��
//		LineCode via1;                     //�o�R1����w��
//		LineCode via2;                     //�o�R2����w��
//		LineCode via3;                     //�o�R3����w��
//		LineCode via4;                     //�o�R4����w��
//		LineCode via5;                     //�o�R5����w��
//		LineCode via6;                     //�o�R6����w��
//		LineCode via7;                     //�o�R7����w��
//		LineCode via8;                     //�o�R8����w��
//
//		Via8Info() : line(0), station_lower(0), station_upper(0), via_num(0) {}
//	};
//
//	struct Via8Data {       // �o�R�W�f�[�^
//		unsigned short via_table_no;      // �o�R�e�[�u��No
//		unsigned short via_info_num;      // �o�R���
//		vector<Via8Info> via8_info;       // �o�R���
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
//// ��挔�o�R�p�^�[���e�[�u���o�R�P�O��^���p
//class ExtraTicketVia10Table {
//public:
//
//	unsigned short via10_table_record_num_; // �o�R�P�O�e�[�u���o�^���R�[�h��
//
//	struct Via10Info {       // �o�R���
//		unsigned short line;      // ����
//		unsigned short station_lower;      // �w������
//		unsigned short station_upper;      // �w�����
//		unsigned short via_num;      // �o�R�w��
//		LineCode via1;                     //�o�R1����w��
//		LineCode via2;                     //�o�R2����w��
//		LineCode via3;                     //�o�R3����w��
//		LineCode via4;                     //�o�R4����w��
//		LineCode via5;                     //�o�R5����w��
//		LineCode via6;                     //�o�R6����w��
//		LineCode via7;                     //�o�R7����w��
//		LineCode via8;                     //�o�R8����w��
//		LineCode via9;                     //�o�R9����w��
//		LineCode via10;                     //�o�R10����w��
//
//		Via10Info() : line(0), station_lower(0), station_upper(0), via_num(0) {}
//	};
//
//	struct Via10Data {       // �o�R�P�O�f�[�^
//		unsigned short via_table_no;      // �o�R�e�[�u��No
//		unsigned short via_info_num;      // �o�R���
//		vector<Via10Info> via10_info;       // �o�R���
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


// ��挔���ʃt�H�[�}�b�g�f�[�^
class ExtraTicketFaceData {
public:

	unsigned short ticketface_data_record_num_; // ���ʃt�H�[�}�b�g�f�[�^�o�^���R�[�h��

	struct Illustration {       // �C���X�g�f�[�^
//		bool is_v_print;  // �����@�󎚗L���@true �L, false ��
		bool is_cm_print;  // �����󎚗L���@true �L, false ��
		unsigned short point_x;     //   X���W
		unsigned short point_y;     //   Y���W
		unsigned short width;     //   ���T�C�Y
		unsigned short height;     //   �c�T�C�Y
		unsigned long illust_offset;     //   �C���X�g�f�[�^�̐擪�A�h���X
//		Illustration() : is_v_print(false), is_cm_print(false), point_x(0), point_y(0), 
		Illustration() : is_cm_print(false), point_x(0), point_y(0), 
			width(0), height(0), illust_offset(0) {}
	};

	struct PrintInfo {          // ���ӕ�
		unsigned short word_line_num;     //   �P�s�̃o�C�g��
		std::string print_word;       // ���ӕ�
		PrintInfo() : word_line_num(0), print_word("") {}
	};

	struct TicketFaceData {       // ���ʃt�H�[�}�b�g�f�[�^
		unsigned short no;     //   �t�H�[�}�b�gNo
		std::string format_code; // �x�[�X���ʃR�[�h�i�`�`�q�j�S�p
		bool is_numbering_print;  // �i���o�����O�󎚗L���@true �L, false ��
		unsigned short illustration_num;     //   �C���X�g�ݒ萔
		vector<Illustration> illustration_info;       // �C���X�g���
		unsigned short print_info_num;     //   ���ӕ��ݒ萔
		vector<PrintInfo> print_info;       // ���ӕ�
		TicketFaceData() : no(0), format_code(""), is_numbering_print(false), illustration_num(0),
			print_info_num(0) {}
	};

	vector<TicketFaceData> ticketface_data_;

	ExtraTicketFaceData();
	virtual ~ExtraTicketFaceData();

	bool internalize(const char*& data);
};


// ��挔�t�H�[�}�b�g�f�[�^���
class TicketFormatData {
public:

	unsigned short extra_ticket_no_;  // ��挔No
    std::string extra_ticket_name_;   // ��挔����
    unsigned short encode_num_;       // ��挔�G���R�[�h����
	std::string v_button_word_jp_1_;  // �����{�^�������@�a��1�i��
	std::string v_button_word_jp_2_;  // �����{�^�������@�a��2�i��
	std::string v_button_word_en_1_;  // �����{�^�������@�p��1�i��
	std::string v_button_word_en_2_;  // �����{�^�������@�p��2�i��
	std::string cm_button_word_jp_1_; // �����{�^�������a��1�i��
	std::string cm_button_word_jp_2_; // �����{�^�������a��2�i��

	struct IssueInfo {       // �������
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

	IssueInfo issue_info_[ISSUE_NUM];       //�������

	TicketFormatData();
	virtual ~TicketFormatData();

};


// ��挔�t�H�[�}�b�g
class ExtraTicketFormatData {
public:

	unsigned short station_code_;   // �w�R�[�h
	Timestamp operate_date_;        // �^�p���t
	unsigned short record_num_;     // ���R�[�h��

	// �f�[�^�w�b�_
	char identify_word_[20]; // ���ʕ�����
	Timestamp effective_start_date_; // �L���J�n���t
	unsigned short tool_version_; // �c�[���o�[�W����
	unsigned short format_version_; // ��挔�t�H�[�}�b�g�f�[�^�o�[�W����
	Timestamp format_create_time_; // ��挔�t�H�[�}�b�g�f�[�^�쐬����
//	unsigned long author_;

	// �C���f�b�N�X�e�[�u��
	unsigned long index_group_table_; // index �O���[�v�e�[�u��
	unsigned long index_format_table_; // index �t�H�[�}�b�g�e�[�u��
//	unsigned long index_via8_table_; // index �o�R�W�p�^�[���e�[�u��
//	unsigned long index_via10_table_; // index �o�R�P�O�p�^�[���e�[�u��
	unsigned long index_ticketface_data_; // index ���ʃt�H�[�}�b�g�f�[�^

	// �O���[�v�e�[�u��
	ExtraTicketGroupTable group_table_;

	// �t�H�[�}�b�g�e�[�u��
	ExtraTicketFormatTable format_table_;

//	// ��挔�o�R�p�^�[���e�[�u���o�R�W������p
//	ExtraTicketVia8Table via8_table_;

//	// ��挔�o�R�p�^�[���e�[�u���o�R�P�O��^���p
//	ExtraTicketVia10Table via10_table_;

	// ��挔���ʃt�H�[�}�b�g�f�[�^
	ExtraTicketFaceData ticketface_format_data_;

	// ��挔�t�H�[�}�b�g�f�[�^���
	vector<TicketFormatData> ticketformat_data_;

	// ����i�P�F�V�A�Q�F���s�j
	int generation_;

	// ���w�̐���w��
	LineCode my_station_code_;

	ExtraTicketFormatData();
	virtual ~ExtraTicketFormatData();

	bool internalize(const char*& data,  int generation);
	void station_code_change(LineCode station_code_in, LineCode& station_code_out);
    bool ticketface_data_bmp_create(const char* data);
    bool ticketface_data_file_create(const char* data);
    bool ticketformat_data_set();
};


// ��挔�f�[�^�i�P���㕪�j
class EachExtraTicketData {
public:
	ExtraTicketMasterData master_;
	ExtraTicketFormatData format_;

	// ����i�P�F�V�A�Q�F���s�j
	int generation_;
	// �}�X�^��
    int ticketnum_;

	EachExtraTicketData();
	virtual ~EachExtraTicketData();

	bool internalize(const char*& data, int generation, int ticketnum);
};


// ��挔�f�[�^
class ExtraTicketData {
public:
//	Prefix         prefix_;                   // �ʐM���w�b�_ ���Ս����Ȃ�
	ComPrefix      comprefix_;                // ���ʃw�b�_
	EachExtraTicketData eachdata_[MASTER_NUM];  // �f�[�^�P�A�Q

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
