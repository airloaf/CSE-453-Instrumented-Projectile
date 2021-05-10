package com.cse453.projectile;

import java.util.ArrayList;

import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.L2CAPConnection;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.microedition.io.Connector;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;

public class Offloader {

	protected Shell shlProjectileOffloader;

	public static ArrayList<RemoteDevice> bluetoothAddresses = new ArrayList<RemoteDevice>();
	public static int curSelectedDevice;
	public static boolean connected = false;

	public static L2CAPConnection connection = null;

	// WIDGETS
	public static Button btnConnectToDevice = null;
	
	/**
	 * Launch the application.
	 * @param args
	 */
	public static void main(String[] args) {
		curSelectedDevice = -1;
		connected = false;
		
		try {
			Offloader window = new Offloader();
			window.open();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static void renderDeviceList(Combo combo)
	{
		// Clear the previous entries
		combo.removeAll();
		curSelectedDevice = -1;
		bluetoothAddresses.clear();
		
		try {
			// Find the bluetooth devices
			LocalDevice device = LocalDevice.getLocalDevice();
			RemoteDevice[] remoteDevice = device.getDiscoveryAgent().retrieveDevices(DiscoveryAgent.PREKNOWN);
			
			// For each bluetooth device, add it to the combo
			for(RemoteDevice d: remoteDevice) {
				combo.add(d.getFriendlyName(false));
				bluetoothAddresses.add(d);
				System.out.println(d);
			}
		}catch(Exception e){}
	}
	
	public static void connectToDevice()
	{
		System.out.println("Connecting to device...");
		if(curSelectedDevice > -1 && curSelectedDevice < bluetoothAddresses.size()) {
			String connURL = "btspp://" + bluetoothAddresses.get(curSelectedDevice).getBluetoothAddress() + ":1";
			System.out.println("Connection string: " + connURL);
			// Create the connection
			try {
//				connection = (L2CAPConnection)
				Connector.open(connURL);
				System.out.println("Connection successful");
				connected = true;
			}catch(Exception e) {
				System.out.println("Connection failed!");
				connection = null;
				connected = false;
			}
			
			setConnectButtonState();
		}
	}
	
	public static void setConnectButtonState()
	{
		if(connected) {
			btnConnectToDevice.setText("Connected");
		}else {
			btnConnectToDevice.setText("Disconnect");
		}
	}
	
	/**
	 * Open the window.
	 */
	public void open() {
		Display display = Display.getDefault();
		createContents();
		shlProjectileOffloader.open();
		shlProjectileOffloader.layout();
		while (!shlProjectileOffloader.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
	}

	/**
	 * Create contents of the window.
	 */
	protected void createContents() {
		shlProjectileOffloader = new Shell();
		shlProjectileOffloader.setSize(871, 385);
		shlProjectileOffloader.setText("Projectile Offloader");
		shlProjectileOffloader.setLayout(new GridLayout(2, true));
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		
		Label lblDevice = new Label(shlProjectileOffloader, SWT.NONE);
		lblDevice.setLayoutData(new GridData(SWT.CENTER, SWT.CENTER, false, false, 1, 1));
		lblDevice.setText("Device");
		
		Combo deviceList = new Combo(shlProjectileOffloader, SWT.READ_ONLY);
		deviceList.addSelectionListener(new SelectionListener() {

			@Override
			public void widgetDefaultSelected(SelectionEvent arg0) {
				curSelectedDevice = -1;
				System.out.println(curSelectedDevice);
			}

			@Override
			public void widgetSelected(SelectionEvent arg0) {
				curSelectedDevice = deviceList.getSelectionIndex();
				System.out.println(curSelectedDevice);
			}
			
		});
		deviceList.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		
		btnConnectToDevice = new Button(shlProjectileOffloader, SWT.NONE);
		btnConnectToDevice.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseDown(MouseEvent e) {
				connectToDevice();
			}
		});
		btnConnectToDevice.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));
		btnConnectToDevice.setText("Connect To Device");
		
		Button btnReloadDevices = new Button(shlProjectileOffloader, SWT.NONE);
		btnReloadDevices.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		btnReloadDevices.setText("Refresh Devices");
		
				btnReloadDevices.addMouseListener(new MouseAdapter() {
					@Override
					public void mouseDown(MouseEvent e) {
						renderDeviceList(deviceList);
					}
				});
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		new Label(shlProjectileOffloader, SWT.NONE);
		
		Button btnSaveData = new Button(shlProjectileOffloader, SWT.NONE);
		GridData gd_btnSaveData = new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1);
		gd_btnSaveData.widthHint = 905;
		btnSaveData.setLayoutData(gd_btnSaveData);
		btnSaveData.setText("Save Data");

		// Render initial device list
		renderDeviceList(deviceList);
	}
}