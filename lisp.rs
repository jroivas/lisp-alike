use std::iter;
use std::collections::LinkedList;

trait ParseData {
    fn walk_tree(&self, level : usize);
    fn leve_str(&self, level : usize) -> String {
        let res : String = iter::repeat(" ").take(level).collect();
        return res;
    }
}

impl ParseData for usize {
    fn walk_tree(&self, level : usize) {
        println!("{}{}", self.leve_str(level), self);
    }
}

impl ParseData for String {
    fn walk_tree(&self, level : usize) {
        println!("{}{}", self.leve_str(level), self);
    }
}

impl ParseData for LinkedList<Box<ParseData>> {
    fn walk_tree(&self, level : usize) {
        println!("{}--", self.leve_str(level));
        for val in self {
            val.walk_tree(level + 1);
        }
    }
}

fn parse(src : &str, mut level : i32) -> (LinkedList<Box<ParseData>>, usize) {
    let mut res : LinkedList<Box<ParseData>> = LinkedList::new();
    let mut partial = String::new();
    let mut idx : usize = 0;

    for (i, c) in src.chars().enumerate() {
        if i >= idx {
        if c == ' ' {
            if !partial.is_empty() {
                //println!("<  {}", partial);
                res.push_back(Box::new(partial));
                partial = String::new();
            }
        } else if c == '(' {
            if !partial.is_empty() {
                //println!("<( {}", partial);
                res.push_back(Box::new(partial));
                partial = String::new();
            }
            let (first, last) = src.split_at(i + 1);
            let (a, b) = parse(last, level + 1);
            idx = i + b + 1;
            res.push_back(Box::new(a));
        } else if c == ')' {
            if !partial.is_empty() {
                res.push_back(Box::new(partial));
                partial = String::new();
            }
            return (res, i + 1);
        } else {
            partial.push(c);
        }
        }
    }

    if !partial.is_empty() {
        res.push_back(Box::new(partial));
    }
    return (res, idx);
}

fn main() {
    let program = "(begin (define r 10) (* pi (* r r)))";
    let mut level : i32 = 0;
    let (parsed, idx) = parse(program, level);
    println!("{}", program);
    parsed.walk_tree(0);
}
