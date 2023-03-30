#include <iostream>
#include <Queue>

class Job {
 public:
  Job(std::string requester, int pages) 
  : id(lastId++), requester(requester), pages(pages) {}

  friend std::ostream& operator<<(std::ostream& out, Job& job) {
    out << "ID : " << job.id << ", Requester : " << job.requester
        << ", Pages : " << job.pages;
    return out;
  }

  friend std::ostream& operator<<(std::ostream& out, const Job& job) {
    out << "ID : " << job.id << ", Requester : " << job.requester
        << ", Pages : " << job.pages;
    return out;
  }

 private:
  static uint32_t lastId;

  uint32_t id = 0;
  std::string requester = "";
  int pages = 0;
};

uint32_t Job::lastId = 0;

class Printer {
  public:
  Printer() = default;

  void Print(std::string requester, int pages) {
    queue.push(new Job(requester, pages));
  };

  void Update() {
    while(true) {
      if (queue.empty()) break;

      Print();
    }
  }

  private:
  void Print() {
    
      std::cout << *queue.front() << std::endl;
      delete queue.front();
      queue.pop();
    
  };
  std::queue<Job*> queue;
};

int main() {
  Printer printer;

  printer.Print("Sim", 53);
  printer.Print("Park", 32);
  printer.Print("Jo", 5321);
  printer.Print("Ha", 2534);

  printer.Update();
}