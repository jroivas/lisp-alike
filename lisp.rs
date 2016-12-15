use std::collections::LinkedList;

trait ParsedString {}
impl ParsedString for String {}
//impl ParsedString for LinkedList;

fn parse(&src: String) -> LinkedList<ParsedString> {
    let mut res : LinkedList<ParsedString> = LinkedList::new();
    let mut partial = String::new();

    for (i, c) in src.chars().enumerate() {
        if c == '(' {
            //mut current = LinkedList::new();
            res.push_back(parse(src.split_at(c + 1)));
        } else if c == ')' {
            if partial {
                res.push_back(partial);
            }
            return res;
        } else {
            partial.push(c);
        }
    }
}

fn main() {
    let program = String::from("(begin (define r 10) (* pi (* r r)))");
    parse(program);
    println!("{}", program);
}
