class StrIter {
public:
  StrIter(const std::string& s) : src_(s), pos_(0), fini_(false)
    {}
  StrIter(const StrIter& c) : src_(c.src_), pos_(0), fini_(false)
    {}
  StrIter& operator=(const StrIter& c)
    { if (this != &c) {
        src_ = c.src_;
        pos_ = 0;
        fini_ = false;
      }
      return *this; }
  virtual ~StrIter()
    {}
  bool next_one(std::string& o, char sep=';')
    { bool ret=false;
      if (!fini_) {
        int strp = src_.find("\"", pos_);
        bool exist_strp=false;
        int np = src_.find(sep, pos_);
        if (strp != std::string::npos && np != std::string::npos &&
            strp < np) {
          strp = src_.find("\"", strp+1);
          np = src_.find(sep, strp+1);
          ++pos_;
          exist_strp=true;
        }
        if (np != std::string::npos) {
          ret = true;
          if (exist_strp) o = src_.substr(pos_, np-pos_-1);
          else            o = src_.substr(pos_, np-pos_);
          if (np == src_.length()) {
            fini_=true;
          }
          pos_ = ++np;
        }
        else {
          if (pos_ < src_.length()) {
            ret = true;
            if (exist_strp) o = src_.substr(pos_, src_.length()-pos_-1);
            else            o = src_.substr(pos_, std::string::npos);
          }
          fini_=true;
        }
      }
      return ret; }
private:
  std::string src_;
  int    pos_;
  bool   fini_;
};