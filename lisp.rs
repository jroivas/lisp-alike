use std::iter;
use std::collections::LinkedList;

struct ParseData {
    value : String ,
    list : LinkedList<ParseData>,
}

fn parse(src : &str, mut level : i32) -> (LinkedList<ParseData>, usize) {
    let mut res : LinkedList<ParseData> = LinkedList::new();
    let mut partial = String::new();
    let mut idx : usize = 0;

    for (i, c) in src.chars().enumerate() {
        if i >= idx {
        if c == ' ' {
            if !partial.is_empty() {
                //println!("<  {}", partial);
                let mut tmp = ParseData { list: LinkedList::new(), value: partial };
                res.push_back(tmp);
                partial = String::new();
            }
        } else if c == '(' {
            if !partial.is_empty() {
                //println!("<( {}", partial);
                let mut tmp = ParseData { list: LinkedList::new(), value: partial };
                res.push_back(tmp);
                partial = String::new();
            }
            let (first, last) = src.split_at(i + 1);
            //println!("( {}", last);
            let (a, b) = parse(last, level + 1);
            idx = i + b + 1;
            //println!("= {} {}", i, idx);
            let mut tmp = ParseData { list: a, value: String::new() };
            res.push_back(tmp);
        } else if c == ')' {
            if !partial.is_empty() {
                //println!("<) {} {} {}", partial, i, idx);
                let mut tmp = ParseData { list: LinkedList::new(), value: partial };
                res.push_back(tmp);
                partial = String::new();
            }
            return (res, i + 1);
        } else {
            partial.push(c);
        }
        }
    }

    if !partial.is_empty() {
        //println!("<  {}", partial);
        let mut tmp = ParseData { list: LinkedList::new(), value: partial };
        res.push_back(tmp);
    }
    return (res, idx);
}

fn dumpTree(data : LinkedList<ParseData>, level : usize) {
    let levelStr : String = iter::repeat(" ").take(level).collect();
    println!("{}--", levelStr);
    for val in data {
        if !val.value.is_empty() {
            println!("{}{}", levelStr, val.value);
        } else {
            dumpTree(val.list, level + 1)
        }
    }
}

fn main() {
    let program = "(begin (define r 10) (* pi (* r r)))";
    let mut level : i32 = 0;
    let (parsed, idx) = parse(program, level);
    println!("{}", program);
    dumpTree(parsed, 0);
}
