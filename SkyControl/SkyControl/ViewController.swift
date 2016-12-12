//
//  ViewController.swift
//  SkyControl
//
//  Created by Clare Bornstein on 12/11/16.
//  Copyright © 2016 Clare Bornstein. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITextFieldDelegate {
    //MARK: Properties
    
    @IBOutlet weak var nameTextField: UITextField!
    
    @IBOutlet weak var serverNameLabel: UILabel!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        nameTextField.delegate = self
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    //MARK: UITextField Delegate
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true
    }
    
    func textFieldDidEndEditing(_ textField: UITextField) {
        serverNameLabel.text = textField.text
    }
    
    //MARK: Actions
    
    @IBAction func setDefaultServerIP(_ sender: UIButton) {
        serverNameLabel.text = "http://127.0.0.1:8080"
    }
    
    //Needs Test File Specified
    @IBAction func testConnection(_ sender: Any) {
        if let url = NSURL(string: serverNameLabel.text!){
            UIApplication.shared.open(url as URL, options: [:], completionHandler: nil)
        }
    }
    //Needs Script Logic
    @IBAction func sunriseCmd(_ sender: Any) {
        if let url = NSURL(string: serverNameLabel.text!){
            UIApplication.shared.open(url as URL, options: [:], completionHandler: nil)
        }
    }
    //Needs Script Logic
    @IBAction func sunsetCmd(_ sender: Any) {
        if let url = NSURL(string: serverNameLabel.text!){
            UIApplication.shared.open(url as URL, options: [:], completionHandler: nil)
        }
    }
    //Needs Script Logic
    @IBAction func nightSkyCmd(_ sender: Any) {
        if let url = NSURL(string: serverNameLabel.text!){
            UIApplication.shared.open(url as URL, options: [:], completionHandler: nil)
        }
    }
    
    
    
}

