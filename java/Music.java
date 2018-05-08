import java.util.List;
import java.util.ArrayList;

 class Instrument {
    public void play() {
        System.out.println("Instument is playing...");
    }
}

 class Wind extends Instrument {
    public void play() {
        System.out.println("Wind is playing...");
    }
}

 class Percussion extends Instrument {
    public void play() {
        System.out.println("Percussion is playing...");
    }
}

public class Music {
    public static void main(String[] args) {
        List<Instrument> instruments = new ArrayList<>();
        instruments.add(new Wind());
        instruments.add(new Percussion());
        for(Instrument instrument : instruments) {
            instrument.play();
        }
    }
}