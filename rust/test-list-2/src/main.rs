extern crate rsc;
use rsc::eval;
use rsc::computer::Computer;

//链表node节点结构体
#[derive(Clone,PartialEq)]
struct Node{
    node:i32,
    next:Option<Box<Node>>
}
impl Node{
    fn new(data:i32)->Node{
        Node{
            node:data,
            next:None,
        }
    }
}
//链表结构体
#[derive(Clone)]
struct NodeList{
    headNode:Option<Box<Node>>,
}
impl NodeList{
    fn new()->NodeList{
        NodeList{
            headNode:None
        }
    }
    //加入链表方法
    fn add(&mut self,data:i32){
        let dataNode=Node::new(data);
        if self.headNode.is_none(){
            self.headNode=Some(Box::new(dataNode));
        }else{
            let headNodeMid=self.headNode.as_mut().expect("wrong");
            //let mut tmpNode=(*tmpNodeMid).clone();
            let mut headNodeRes=&mut (*headNodeMid);
            //let mut headNodeTmp=headNodeRes;
            while headNodeRes.next.is_some(){
                let  headNodeTmp =headNodeRes.next.as_mut().expect("wrong");
                let  headNodeTmp1=&mut (*headNodeTmp);
                headNodeRes=headNodeTmp1;
            }
            (*headNodeRes).next=Some(Box::new(dataNode));
        }
    }
    //打印链表上的节点
    fn showAll(&mut self){
        if self.headNode.is_none(){
            print!("no data")
        }else{
            let tmpNodeMid=self.headNode.as_mut().expect("wrong");
            let mut headNodeRes=&mut (*tmpNodeMid);
            while headNodeRes.next.is_some(){
                println!("datanode is {}",headNodeRes.node);
                let mut headNode1 =headNodeRes.next.as_mut().expect("wrong");
                let mut headNodeTmp1=&mut (*headNode1);
                headNodeRes=headNodeTmp1;

            }
            println!("datanode is {}",headNodeRes.node);
        }
    }

}
//主函数
fn main() {
    let mut list=NodeList::new();
    list.add(1);
    list.add(2);
    list.add(3);
    list.add(4);
    list.add(5);
    list.add(6);
    list.showAll();
    let a:i32 = 1;
    let mut c = Computer::<f64>::default();

    println!("result = {}", c.eval("3+4+5+6*7/3-2.00*3.0/2.0").unwrap());
    // println!("result = {}", eval::<f32>("3+4+5+6*7/3-2.00*3.0").unwrap());
}
// 原文链接：https://blog.csdn.net/q291938895/article/details/109015041

