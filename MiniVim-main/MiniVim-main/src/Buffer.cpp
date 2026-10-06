#include <fstream>
#include<utility>

#include "Buffer.hpp"

namespace sjtu {



Buffer::Buffer(const std::filesystem::path& path):path_(path){
    if(!path_.empty() && std::filesystem::exists(path_)){
        std::ifstream input(path,std::ios::binary);
        if(!input){
            throw std::runtime_error("cannot open: " + path.string());
        }
        std::string line;
        while(std::getline(input,line)){
            lines_.push_back(line);
        }
    }

        empty_file_ = lines_.empty();
        if (lines_.empty()) {
        lines_.push_back("");
    }
        saved_revision_ = revision_;
    } 
    
    //从path指向的文件构造Buffer,你需要打开文件并且把文件内容填充进Buffer,并正确初始化一些状态.
    //注意path可能为空的边界情况







Buffer::Buffer(std::vector<std::string> lines, std::filesystem::path path) : lines_(std::move(lines)),path_(std::move(path)) {
    empty_file_=lines_.empty();
    if(lines_.empty()) {
        lines_.push_back("");
    }
    saved_revision_=revision_;
}



std::size_t Buffer::GetLineCount() const {
    //返回文件行数
    return lines_.size();
}

const std::string& Buffer::GetLineAt(std::size_t row) const {
    //返回第row行的内容
    return lines_[row];
}


std::string Buffer::GetDisplayName() const {
    //返回文件名,若是新文件,返回"[No Name]"
    if(path_.empty()){
        return "[No Name]";
    }
    else return path_.string();
}

bool Buffer::IsModified() const {
    //返回文件和上次保存比起来是否被修改过
    return revision_ != saved_revision_;
}

void Buffer::InsertCharacter(std::size_t row, std::size_t column, char value) {
    //在第row行第col列插入一个value, 注意越界检查
    if(column>lines_[row].size()){
        throw std::out_of_range("insert column wrong");
    }
    lines_[row].insert(column,1,value);
    empty_file_=false;
    revision_=++next_revision_;
}



void Buffer::EraseCharacter(std::size_t row, std::size_t column) {
   //在第row行第col列删除一个value
    if(row >= lines_.size()) {
    return;
   }
    if(column >= lines_[row].size()){ 
        return;
    }
    lines_[row].erase(column,1);
    empty_file_=false;
    revision_=++next_revision_;
}






void Buffer::SplitLine(std::size_t row, std::size_t column) {
    //在第row行第col列分割,即在此处敲了回车键
    if(row >= lines_.size()) {
    return;
   }
    const auto tail = lines_[row].substr(column);
    lines_[row].erase(column);
    lines_.insert(lines_.begin()+row+1,tail);
    empty_file_ = false;
    revision_ = ++next_revision_;
}





void Buffer::JoinLine(std::size_t row) {
   //把第row + 1行合并进第row行
    if(row >= lines_.size()-1) {
    return;
   }
    const auto substring = lines_[row+1];
    lines_[row] += substring;
    lines_.erase(lines_.begin()+row+1);
    empty_file_ = false;
    revision_ = ++next_revision_;
}






void Buffer::Save() {
   //把文件内容保存, 直接调用WriteTo方法
   WriteTo(path_);
   saved_revision_ = revision_;
}

void Buffer::SaveAs(const std::filesystem::path& path) {
    WriteTo(path);
    path_ = path;
    saved_revision_ = revision_;
}




void Buffer::WriteTo(const std::filesystem::path& path) const {
   if (path.empty()){ 
    throw std::runtime_error("No file name");
   }
   std::ofstream output(path,std::ios::binary | std::ios::trunc);
   if(!output){
    throw std::runtime_error("can not write:" + path.string());
   }
   if(!empty_file_){
    for(const auto& line:lines_){
        output<<line<<std::endl;
    }
}
    output.close();
    if(!output){
     throw std::runtime_error("write failed:" + path.string());
    }
}






} // namespace sjtu
