
import UIKit

class StartViewController: UIViewController {

    @IBOutlet weak var btnNewGame: UIButton!
    @IBOutlet weak var btnPause: UIButton!
    @IBOutlet weak var btnExit: UIButton!
    override func viewDidLoad() {
        super.viewDidLoad()

    }
    @IBAction func actionPause(_ sender: Any) {
    }
    
    @IBAction func actionNewgame(_ sender: Any) {
        let vc = self.storyboard?.instantiateViewController(withIdentifier: "ViewController") as! ViewController
        vc.modalPresentationStyle = .currentContext
        self.present(vc, animated: true, completion: nil)
    }
    
    @IBAction func actionExit(_ sender: Any) {
        exit(0)
    }
    
}
