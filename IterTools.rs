#![forbid(unsafe_code)]

use std::{cell::RefCell, collections::VecDeque, rc::Rc};

pub struct LazyCycle<I>
where
    I: Iterator,
    I::Item: Clone,
{
    iter: I,
    storage: Vec<I::Item>,
    ind: usize,
    ended: bool,
}

impl<I> LazyCycle<I>
where
    I: Iterator,
    I::Item: Clone,
{
    pub fn new(it: I) -> Self {
        Self {
            iter: it,
            storage: Vec::new(),
            ind: 0,
            ended: false,
        }
    }
}

impl<I> Iterator for LazyCycle<I>
where
    I: Iterator,
    I::Item: Clone,
{
    type Item = I::Item;
    fn next(&mut self) -> Option<I::Item> {
        if !self.ended {
            let val = self.iter.next();
            if val.is_none() {
                self.ended = true;
                self.ind = 1;
                match self.storage.len() {
                    ln if ln > 0 => Some(self.storage[0].clone()),
                    _ => None,
                }
            } else {
                // self.storage.push(val.unwrap());
                if let Some(el) = val {
                    self.storage.push(el);
                }
                // match self.storage.last() {
                //     Some(el) => Some(el.clone()),
                //     _ => None,
                // }
                self.storage.last().cloned()
            }
        } else {
            if self.ind >= self.storage.len() {
                self.ind = 0;
            }
            // let val match self.storage.get(self.ind) {
            //     Some(el) => Some(el.clone()),
            //     _ => None,
            // }
            let val = self.storage.get(self.ind).cloned();
            self.ind += 1;
            val
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

pub struct Extract<I: Iterator> {
    iter: I,
    storage: VecDeque<Option<I::Item>>,
    stop_ind: usize,
}

impl<I: Iterator> Extract<I> {
    pub fn new(it: I, n: usize) -> (Option<I::Item>, Self) {
        let mut obj = Self {
            iter: it,
            storage: VecDeque::new(),
            stop_ind: n,
        };
        for _i in 0..n {
            obj.storage.push_back(obj.iter.next());
        }

        (obj.iter.next(), obj)
    }

    pub fn get_skiped_ind(&self) -> usize {
        self.stop_ind
    }
}

impl<I: Iterator> Iterator for Extract<I> {
    type Item = I::Item;
    fn next(&mut self) -> Option<I::Item> {
        if self.storage.is_empty() {
            self.iter.next()
        } else {
            self.storage.pop_front().unwrap()
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

pub struct Tee<I>
where
    I: Iterator,
    I::Item: Clone,
{
    iter: Rc<RefCell<I>>,
    storage: Rc<RefCell<VecDeque<I::Item>>>,
    ind_first: Rc<RefCell<usize>>,
    ind_second: Rc<RefCell<usize>>,
    num: usize,
    ended: Rc<RefCell<bool>>,
}

impl<I> Tee<I>
where
    I: Iterator,
    I::Item: Clone,
{
    pub fn new(it: I) -> Self {
        Self {
            iter: Rc::new(RefCell::new(it)),
            storage: Rc::new(RefCell::new(VecDeque::new())),
            ind_first: Rc::new(RefCell::new(0)),
            ind_second: Rc::new(RefCell::new(0)),
            num: 0,
            ended: Rc::new(RefCell::new(false)),
        }
    }

    pub fn new_start_copy(it: &Tee<I>) -> Self {
        Self {
            iter: it.iter.clone(),
            storage: it.storage.clone(),
            ind_first: it.ind_first.clone(),
            ind_second: it.ind_second.clone(),
            num: 1,
            ended: it.ended.clone(),
        }
    }
}

impl<I> Iterator for Tee<I>
where
    I: Iterator,
    I::Item: Clone,
{
    type Item = I::Item;

    fn next(&mut self) -> Option<I::Item> {
        let cur_num = self.num;
        let (mut old_cur_ind, mut old_ano_ind) = match cur_num {
            0 => (self.ind_first.borrow_mut(), self.ind_second.borrow_mut()),
            1 => (self.ind_second.borrow_mut(), self.ind_first.borrow_mut()),
            _ => panic!(),
        };

        let old_len = self.storage.borrow().len();

        if *old_cur_ind < *old_ano_ind {
            if *old_ano_ind > 0 {
                *old_ano_ind -= 1;
            }
            if old_len > 0 {
                self.storage.borrow_mut().pop_front()
            } else {
                None
            }
        } else if !*(self.ended.borrow()) {
            *old_cur_ind += 1;
            match self.iter.borrow_mut().next() {
                Some(el) => self.storage.borrow_mut().push_back(el),
                _ => {
                    (*(self.ended.borrow_mut())) = true;
                    return None;
                }
            };
            // match self.storage.borrow_mut().back() {
            //     Some(el) => Some(el.clone()),
            //     _ => None,
            // }
            self.storage.borrow_mut().back().cloned()
        } else {
            None
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

pub struct GroupBy<I, F, V>
where
    I: Iterator,
    F: FnMut(&I::Item) -> V,
    V: Eq,
{
    iter: I,
    func: F,
    prev: Option<I::Item>,
    filled: bool,
}

impl<I, F, V> GroupBy<I, F, V>
where
    I: Iterator,
    F: FnMut(&I::Item) -> V,
    V: Eq,
{
    pub fn new(iter: I, func: F) -> Self {
        Self {
            iter,
            func,
            prev: None,
            filled: false,
        }
    }
}

impl<I, F, V> Iterator for GroupBy<I, F, V>
where
    I: Iterator,
    F: FnMut(&I::Item) -> V,
    V: Eq,
{
    type Item = (V, Vec<I::Item>);
    fn next(&mut self) -> Option<(V, Vec<I::Item>)> {
        if self.filled {
            self.filled = true;
            return None;
        }
        //-------
        else if self.prev.is_none() {
            let val = self.iter.next();
            // if val.is_none() {
            //     return None;
            // }
            val.as_ref()?;
            let mut buf: Vec<I::Item> = Vec::new();
            let val_in = val.unwrap();
            let val_eq = (self.func)(&val_in);
            buf.push(val_in);
            loop {
                let new_val = self.iter.next();
                if new_val.is_none() {
                    self.filled = true;
                    return Some((val_eq, buf));
                }
                if let Some(new_val_in) = new_val {
                    if (self.func)(&new_val_in) != val_eq {
                        self.prev = Some(new_val_in);
                        return Some((val_eq, buf));
                    } else {
                        buf.push(new_val_in);
                    }
                }
            }
        }
        //-------
        else {
            let mut buf: Vec<I::Item> = Vec::new();

            let spr = self.prev.take();

            if let Some(vv) = spr {
                let val_eq = (self.func)(&vv);
                buf.push(vv);
                loop {
                    let new_val = self.iter.next();
                    if new_val.is_none() {
                        self.filled = true;
                        return Some((val_eq, buf));
                    }
                    if let Some(new_val_in) = new_val {
                        if (self.func)(&new_val_in) != val_eq {
                            self.prev = Some(new_val_in);
                            return Some((val_eq, buf));
                        } else {
                            buf.push(new_val_in);
                        }
                    }
                }
            }
        }
        //-------

        None
    }
}

////////////////////////////////////////////////////////////////////////////////

pub trait ExtendedIterator: Iterator {
    fn lazy_cycle(self) -> LazyCycle<Self>
    where
        Self: Sized,
        Self::Item: Clone,
    {
        LazyCycle::new(self)
    }

    fn extract(self, index: usize) -> (Option<Self::Item>, Extract<Self>)
    where
        Self: Sized,
    {
        Extract::new(self, index)
    }

    fn tee(self) -> (Tee<Self>, Tee<Self>)
    where
        Self: Sized,
        Self::Item: Clone,
    {
        let tee1: Tee<Self> = Tee::new(self);
        let tee2: Tee<Self> = Tee::new_start_copy(&tee1);
        (tee1, tee2)
    }

    fn group_by<F, V>(self, func: F) -> GroupBy<Self, F, V>
    where
        Self: Sized,
        F: FnMut(&Self::Item) -> V,
        V: Eq,
    {
        GroupBy::new(self, func)
    }
}

impl<T: Iterator> ExtendedIterator for T {}
