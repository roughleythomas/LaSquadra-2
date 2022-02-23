import GLKit

extension ViewController: GLKViewControllerDelegate
{
    func glkViewControllerUpdate(_ controller: GLKViewController)
    {
        glesRenderer.update();
        
    }
}

class ViewController: GLKViewController
{
    
    private var context: EAGLContext?
    private var glesRenderer: Renderer!
    var initialCenter = CGPoint()


    private func setupGL()
    {
        context = EAGLContext(api: .openGLES3)
        EAGLContext.setCurrent(context)
        if let view = self.view as? GLKView,
            let context = context
            {
                view.context = context
                delegate = self as GLKViewControllerDelegate
                
                glesRenderer = Renderer()
                glesRenderer.setup(view)
                glesRenderer.loadModels()
            }
    }
    
    private var buttonPause = UIButton()
    private var buttonTittle = Bool()

    override func viewDidLoad()
    {
        super.viewDidLoad()
        setupGL()
        let doubleTap = UITapGestureRecognizer(target: self, action: #selector(self.doDoubleTap(_:)))
        doubleTap.numberOfTapsRequired = 2;
        view.addGestureRecognizer(doubleTap)
        

        let pan = UIPanGestureRecognizer(target: self, action: #selector(doPan(_:)))
        view.addGestureRecognizer(pan)
        
    
        let pinch = UIPinchGestureRecognizer(target: self, action: #selector(doPinch(_:)))
        self.view.addGestureRecognizer(pinch)
        

        

        let button = UIButton(frame: CGRect(x: 100, y: 75, width: 100, height: 50))
        button.setTitle("Reset", for: .normal)
        button.addTarget(self, action: #selector(buttPress), for: .touchUpInside)
        self.view.addSubview(button)
 
        buttonPause.frame = CGRect(x: 100, y: self.view.frame.maxY-200, width: 100, height: 50)
        buttonPause.setTitle("Pause", for: .normal)
        buttonPause.addTarget(self, action: #selector(onTapPause), for: .touchUpInside)
        self.view.addSubview(buttonPause)
        
buttonTittle = false
        
    }
    
    override func glkView(_ view: GLKView, drawIn rect: CGRect)
    {
        glesRenderer.draw(rect);
        let labelRect = CGRect(x: 30, y: 100, width: 250, height: 100)
        let label = UILabel(frame: labelRect)
        label.textAlignment = .center
        label.textColor = UIColor.red;
        label.numberOfLines = 2;
        label.tag = 1;
        
  
        let px = round(glesRenderer.panX * 10) / 10.0;
        let py = round(glesRenderer.panY * 10) / 10.0;
        let rx = round(glesRenderer.rotX * 10) / 10.0;
        let ry = round(glesRenderer.rotY * 10) / 10.0;
        
        label.text = "Position: \(px), \(py), -5.0 \n Rotation: \(rx), \(ry), 0.0"
        print(label.text!)
        view.viewWithTag(1)?.removeFromSuperview()
        self.view.addSubview(label)
        
    }
    

    @objc func doDoubleTap(_ sender: UITapGestureRecognizer) {

        
        glesRenderer.isRed = !glesRenderer.isRed;
 
        glesRenderer.isRotating = !glesRenderer.isRotating;
        
        
    }
    

    @objc private func doPan(_ sender: UIPanGestureRecognizer) {

        
        if (glesRenderer.isRotating == false) {
            if (sender.numberOfTouches == 1) {
                let changedDistance: CGPoint = sender.translation(in: view)
                
                glesRenderer.rotX = changedDistance.x
                glesRenderer.rotY = changedDistance.y
       
            }
            
        
            if (sender.numberOfTouches == 2) {
                glesRenderer.panX = sender.translation(in: view).x
                glesRenderer.panY = sender.translation(in: view).y
            }
        }
    }
    

    @objc func doPinch(_ sender: UIPinchGestureRecognizer) {
        glesRenderer.scale = sender.scale;
    }
    

    @objc func buttPress(sender: UIButton!) {
        glesRenderer.rotX = 0
        glesRenderer.rotY = 0
        glesRenderer.panX = 0
        glesRenderer.panY = 0
        print("reset");
onTapPause()
    }
    

    
}

extension ViewController{
  @objc func onTapPause() {
        
        let alertController  = UIAlertController(title: "Choose action", message: "", preferredStyle: .actionSheet)
        
        alertController.addAction(UIAlertAction(title: buttonTittle ? "Resume" : "pause", style: .default, handler: tapResume))
        
        alertController.addAction(UIAlertAction(title: "New Game", style: .default, handler: tapNewGame))
        alertController.addAction(UIAlertAction(title: "Exit", style: UIAlertAction.Style.cancel, handler: tapExit))
        
 
        alertController.popoverPresentationController?.sourceView = self.view
        alertController.popoverPresentationController?.permittedArrowDirections = UIPopoverArrowDirection()
        alertController.popoverPresentationController?.sourceRect = CGRect(x: self.view.bounds.midX, y: self.view.bounds.maxY, width: 0, height: 0)
        
        self.present(alertController, animated: true, completion: nil)
    }
    
    
    func tapResume(action: UIAlertAction){
        buttonTittle.toggle()
        buttonPause.setTitle(buttonTittle ? "Resume" : "pause", for: .normal)
        glesRenderer.isRed = !glesRenderer.isRed;

        glesRenderer.isRotating = !glesRenderer.isRotating;

        
    }
    
    func tapNewGame(action: UIAlertAction){
     setupGL()
        buttonPause.frame = CGRect(x: 100, y: self.view.frame.maxY-200, width: 100, height: 50)
        buttonPause.setTitle("Pause", for: .normal)
        buttonPause.addTarget(self, action: #selector(onTapPause), for: .touchUpInside)
        self.view.addSubview(buttonPause)
        
buttonTittle = false
        
    }
    
    func tapExit(action: UIAlertAction){
      exit(0)
        
    }
}
