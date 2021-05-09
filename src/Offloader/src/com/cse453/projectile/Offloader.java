package com.cse453.projectile;

import java.util.ArrayList;

import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;

import org.eclipse.swt.SWT;
import org.eclipse.swt.events.MouseAdapter;
import org.eclipse.swt.events.MouseEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Combo;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.wb.swt.SWTResourceManager;

public class Offloader {

	protected Shell shlProjectileOffloader;

	public static ArrayList<String> bluetoothAddresses = new ArrayList<String>();
	public static int curSelectedDevice;
	
	/**
	 * Launch the application.
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("Hello World!");
	
		curSelectedDevice = -1;
		
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
				bluetoothAddresses.add(d.getBluetoothAddress());
			}
		}catch(Exception e){}
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
		lblDevice.setFont(SWTResourceManager.getFont("Segoe UI", 20, SWT.NORMAL));
		lblDevice.setLayoutData(new GridData(SWT.CENTER, SWT.CENTER, false, false, 1, 1));
		lblDevice.setText("Device");
		
		Combo deviceList = new Combo(shlProjectileOffloader, SWT.READ_ONLY);
		deviceList.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
		
		Button btnConnectToDevice = new Button(shlProjectileOffloader, SWT.NONE);
		btnConnectToDevice.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, false, false, 1, 1));
		btnConnectToDevice.setFont(SWTResourceManager.getFont("Segoe UI", 15, SWT.BOLD));
		btnConnectToDevice.setText("Connect To Device");
		
		Button btnReloadDevices = new Button(shlProjectileOffloader, SWT.NONE);
		btnReloadDevices.setFont(SWTResourceManager.getFont("Segoe UI", 15, SWT.BOLD));
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
		btnSaveData.setFont(SWTResourceManager.getFont("Segoe UI", 15, SWT.BOLD));
		GridData gd_btnSaveData = new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1);
		gd_btnSaveData.widthHint = 905;
		btnSaveData.setLayoutData(gd_btnSaveData);
		btnSaveData.setText("Save Data");

		// Render initial device list
		renderDeviceList(deviceList);
	}
}
