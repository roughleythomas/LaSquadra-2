//
//  Copyright © Borna Noureddin. All rights reserved.
//

import GLKit    // use GLKit to treat the iOS display as one that can receive GL draw commands

// This enables using the GLKit update method to call our own update
extension ViewController: GLKViewControllerDelegate {
    func glkViewControllerUpdate(_ controller: GLKViewController) {
        glesRenderer.update();
        
    }
}

class ViewController: GLKViewController {
    
    private var context: EAGLContext?       // EAGL context for GL draw commands
    private var glesRenderer: Renderer!     // our own C++ GLES renderer object
    var initialCenter = CGPoint();  // The initial center point of the view.
    var upHeld = false;
    var rightHeld = false;
    var downHeld = false;
    var leftHeld = false;
    

    
    private func setupGL() {
        // Set up the GL context and initialize and setup our GLES renderer object
        //1, Allocate a EAGLContext object and initialize a context with a specific version.
        context = EAGLContext(api: .openGLES3)
        EAGLContext.setCurrent(context)
        
        // Set view to size of screen I guess
        //glkView.frame = CGRect(x: 0.0, y: 0.0, width: ScreenSize.width, height: ScreenSize.height)
        
        if let view = self.view as? GLKView, let context = context {
            view.context = context
            delegate = self as GLKViewControllerDelegate
            glesRenderer = Renderer()
            glesRenderer.setup(view)
            glesRenderer.loadModels()
        }
    }
    
    
    override func viewDidLoad() {
        // This gets called as soon as the view is loaded
        super.viewDidLoad()
        setupGL()   // call this to set up our GL environment
        	
//        // Set up a double-tap ge*sture recognizer
//        let doubleTap = UITapGestureRecognizer(target: self, action: #selector(self.doDoubleTap(_:)))
//        doubleTap.numberOfTapsRequired = 2;
//        view.addGestureRecognizer(doubleTap)
        
//        //Set up a pan gesture recognizer
//        let pan = UIPanGestureRecognizer(target: self, action: #selector(doPan(_:)))
//        view.addGestureRecognizer(pan)
        
        //Set up a pinch gesture recognizer
        let pinch = UIPinchGestureRecognizer(target: self, action: #selector(doPinch(_:)))
        self.view.addGestureRecognizer(pinch)
        
        // make the button
        let button = UIButton(frame: CGRect(x: 100, y: 75, width: 100, height: 50))
        button.setTitle("Reset", for: .normal)
        button.addTarget(self, action: #selector(buttPress), for: .touchUpInside)
        self.view.addSubview(button)
        
        // MOVEMENT BUTTONS
        let upButton = UIButton(frame: CGRect(x: 50, y: 400, width: 50, height: 50))
        let upArrow = UIImage(named: "upArrow")
        upButton.setImage(upArrow, for: UIControl.State())
        upButton.addTarget(self, action: #selector(upPress), for: .touchDown)
        upButton.addTarget(self, action: #selector(stopUp(_:)), for: .touchUpInside)
        self.view.addSubview(upButton)
        
        let rightButton = UIButton(frame: CGRect(x: 100, y: 450, width: 50, height: 50))
        let rightArrow = UIImage(named: "upArrow")
        rightButton.setImage(rightArrow, for: UIControl.State())
        rightButton.addTarget(self, action: #selector(rightPress), for: .touchDown)
        rightButton.addTarget(self, action: #selector(stopRight(_:)), for: .touchUpInside)
        self.view.addSubview(rightButton)
        
        let downButton = UIButton(frame: CGRect(x: 50, y: 500, width: 50, height: 50))
        let downArrow = UIImage(named: "downArrow")
        downButton.setImage(downArrow, for: UIControl.State())
        downButton.addTarget(self, action: #selector(downPress), for: .touchDown)
        downButton.addTarget(self, action: #selector(stopDown(_:)), for: .touchUpInside)
        self.view.addSubview(downButton)
        
        let leftButton = UIButton(frame: CGRect(x: 0, y: 450, width: 50, height: 50))
        let leftArrow = UIImage(named: "leftArrow")
        leftButton.setImage(leftArrow, for: UIControl.State())
        leftButton.addTarget(self, action: #selector(leftPress), for: .touchDown)
        leftButton.addTarget(self, action: #selector(stopLeft(_:)), for: .touchUpInside)
        self.view.addSubview(leftButton)
        
    }
    
    @IBAction func stopUp(_ sender: UIButton) {
            upHeld = true
        }
    @IBAction func stopRight(_ sender: UIButton) {
            rightHeld = true
        }
    @IBAction func stopDown(_ sender: UIButton) {
            downHeld = true
        }
    @IBAction func stopLeft(_ sender: UIButton) {
            leftHeld = true
        }
    
    override func glkView(_ view: GLKView, drawIn rect: CGRect) {
        glesRenderer.draw(rect);    // use our custom GLES renderer object to make the actual GL draw calls
        
        // make label
        let labelRect = CGRect(x: 30, y: 100, width: 250, height: 100)
        let label = UILabel(frame: labelRect)
        label.textAlignment = .center
        label.textColor = UIColor.red;
        label.numberOfLines = 2;
        label.tag = 1;
        
        // round to one decimal place
        let px = round(glesRenderer.panX * 10) / 10.0;
        let py = round(glesRenderer.panY * 10) / 10.0;
        let rx = round(glesRenderer.rotX * 10) / 10.0;
        let ry = round(glesRenderer.rotY * 10) / 10.0;
        
        label.text = "Position: \(px), \(py), -5.0 \n Rotation: \(rx), \(ry), 0.0"
        
        view.viewWithTag(1)?.removeFromSuperview()
        self.view.addSubview(label)
        
    }
    
    @objc func doDoubleTap(_ sender: UITapGestureRecognizer) {
        // Handle double tap gesture
        glesRenderer.isRed = !glesRenderer.isRed;
        // You can add additional things here, for example to toggle whether a cube auto-rotates
        glesRenderer.isRotating = !glesRenderer.isRotating;
    }
    
    @objc private func doPan(_ sender: UIPanGestureRecognizer) {
        //view.center = sender.location(in: view)
        
        if (glesRenderer.isRotating == false) {
            if (sender.numberOfTouches == 1) { // Do single finger pan
                let changedDistance: CGPoint = sender.translation(in: view)
                
                glesRenderer.rotX = changedDistance.x
                glesRenderer.rotY = changedDistance.y
                // finger drag has no z axis
            }
            
        
            if (sender.numberOfTouches == 2) { // Do double finger pan
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
    }
    
    @objc func upPress(sender: UIButton!) {
        var continueIt = false
        DispatchQueue.global(qos: .userInitiated).async {
            repeat {
                continueIt = !self.upHeld
                self.glesRenderer.panY += 0.0000001
            } while continueIt
        }
        upHeld = false
    }
    
    @objc func rightPress(sender: UIButton!) {
        var continueIt = false
        DispatchQueue.global(qos: .userInitiated).async {
            repeat {
                continueIt = !self.rightHeld
                self.glesRenderer.panX += 0.0000001
            } while continueIt
        }
        rightHeld = false
    }
    
    @objc func downPress(sender: UIButton!) {
        var continueIt = false
        DispatchQueue.global(qos: .userInitiated).async {
            repeat {
                continueIt = !self.downHeld
                self.glesRenderer.panY -= 0.0000001
            } while continueIt
        }
        downHeld = false
    }
    
    @objc func leftPress(sender: UIButton!) {
        var continueIt = false
        DispatchQueue.global(qos: .userInitiated).async {
            repeat {
                continueIt = !self.leftHeld
                self.glesRenderer.panX -= 0.0000001
            } while continueIt
        }
        leftHeld = false
    }

    
}
