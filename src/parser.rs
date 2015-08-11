pub struct Parser {
    src: Vec<u8>,
    pos: usize,
    lino: usize,
}

impl Parser {
    pub fn new(src: String) -> Parser {
        return Parser {
            src: src.as_bytes(),
            pos: 0,
            lino: 1,
        };
    }

    fn have_more(&self) -> bool {
        return self.pos < self.src.len();
    }

    fn pop(&self, n: isize) {
        self.pos += 1;
    }

    fn peek(&self, idx: isize) -> char {
        if self.pos + idx < self.src.len() {
            return self.src[self.pos + idx];
        } else {
            return -1;
        }
    }
}
