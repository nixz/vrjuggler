/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.wizard.cluster;

import java.util.Iterator;
import java.io.IOException;
import javax.swing.*;
import java.awt.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.wizard.*;
import java.awt.event.*;
import javax.swing.border.*;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

public class CreateClusteredSimDevicesPanel extends JPanel
{
  BorderLayout baseLayout = new BorderLayout();
  JPanel centerPnl = new JPanel();

  private DefaultComboBoxModel lstHeadNodes = new DefaultComboBoxModel();
  private DefaultComboBoxModel lstWandNodes = new DefaultComboBoxModel();
  private DefaultComboBoxModel lstCameraNodes = new DefaultComboBoxModel();
  // Contains all sim display windows and the display system after
  // copying the config files and before writing all machine specific details
  private java.util.List mDisplayElementList = new java.util.ArrayList();
  private java.util.List mDisplaySystemElementList = new java.util.ArrayList();

  private String mFileSourceName = null;

  private ConfigContext mContext = null;
  private ConfigBroker mBroker = null;
  private TitledBorder titledBorder1;
  private JComboBox cbSimCameraKeyboard = new JComboBox();
  private JComboBox cbHeadKeyboard = new JComboBox();
  private JComboBox cbWandKeyboard = new JComboBox();
  private JLabel lblHeadKeyboard = new JLabel();
  private JLabel lblSimCameraKeyboard = new JLabel();
  private JLabel lblWandKeyboard = new JLabel();
  private Component component1;
  private Component component2;
  private Component component3;
  private Component component4;
  private Component component5;
  private Component component6;
  private Component component7;
  private Component component8;
  private GridLayout gridLayout1 = new GridLayout();

  public CreateClusteredSimDevicesPanel()
  {
     try
     {
       jbInit();
     }
     catch(Exception e)
     {
        e.printStackTrace();
     }
     try
     {
       jLabel2.setIcon(new ImageIcon(this.getClass().getClassLoader().getResource("org/vrjuggler/vrjconfig/wizard/cluster/images/ClusterIcon.png")));
     }
     catch(NullPointerException exp)
     {
       jLabel2.setText("");
     }
  }

  private void jbInit() throws Exception
  {
    titledBorder1 = new TitledBorder("");
    setLayout(baseLayout);
    component1 = Box.createVerticalStrut(1);
    component2 = Box.createVerticalStrut(1);
    component3 = Box.createVerticalStrut(1);
    component4 = Box.createVerticalStrut(1);
    component5 = Box.createVerticalStrut(1);
    component6 = Box.createVerticalStrut(1);
    component7 = Box.createVerticalStrut(1);
    component8 = Box.createVerticalStrut(1);

    // Head Keyboard JComboBox
    cbHeadKeyboard.setActionCommand("headChanged");
    cbHeadKeyboard.setModel(lstHeadNodes);
    cbHeadKeyboard.addActionListener(new java.awt.event.ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
        updateSimDevices(e);
      }
    });
    // Wand Keyboard JComboBox
    cbWandKeyboard.setActionCommand("wandChanged");
    cbWandKeyboard.setModel(lstWandNodes);
    cbWandKeyboard.addActionListener(new java.awt.event.ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
        updateSimDevices(e);
      }
    });
    // Sim Camera Controls Keyboard JComboBox
    cbSimCameraKeyboard.setActionCommand("cameraChanged");
    cbSimCameraKeyboard.setModel(lstCameraNodes);
    cbSimCameraKeyboard.addActionListener(new java.awt.event.ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
        updateSimDevices(e);
      }
    });

    lblHeadKeyboard.setToolTipText("");
    lblHeadKeyboard.setText("Head Keyboard");
    lblSimCameraKeyboard.setText("Sim Camera Keyboard");
    lblWandKeyboard.setText("Wand Keyboard");
    gridLayout1.setColumns(2);
    gridLayout1.setRows(7);
    directionsPanel.setBorder(BorderFactory.createEtchedBorder());
    directionsPanel.setLayout(borderLayout1);
    jLabel2.setIcon(new ImageIcon("/home/users/aronb/Source/ClusterWizard/images/ClusterIcon.png"));
    lblTitle.setFont(new java.awt.Font("Serif", 1, 20));
    lblTitle.setHorizontalAlignment(SwingConstants.LEFT);
    lblTitle.setText("Set Device Hosts");
    lblDirections.setText("Select the cluster node that you want to share" +
                          " each of the following keyboards from.");
    baseLayout.setHgap(5);
    baseLayout.setVgap(5);
    centerPnl.setLayout(gridLayout1);
    this.add(centerPnl, BorderLayout.CENTER);

    // Adding the comboboxes and making them spaced out
    centerPnl.add(component1, null);
    centerPnl.add(component2, null);
    centerPnl.add(lblHeadKeyboard, null);
    centerPnl.add(cbHeadKeyboard, null);
    centerPnl.add(component3, null);
    centerPnl.add(component4, null);
    centerPnl.add(lblWandKeyboard, null);
    centerPnl.add(cbWandKeyboard, null);
    centerPnl.add(component5, null);
    centerPnl.add(component6, null);
    centerPnl.add(lblSimCameraKeyboard, null);
    centerPnl.add(cbSimCameraKeyboard, null);
    centerPnl.add(component7, null);
    centerPnl.add(component8, null);
    jPanel2.add(lblTitle, null);
    jPanel2.add(lblDirections, null);
    directionsPanel.add(jPanel3, BorderLayout.EAST);
    jPanel3.add(jLabel2, null);
    directionsPanel.add(jPanel2, BorderLayout.CENTER);
    this.add(directionsPanel, BorderLayout.NORTH);
  }

  public void setFileSourceName(String file_name)
  {
    mFileSourceName = file_name;
  }
  public String getFileSourceName()
  {
    return(mFileSourceName);
  }


  public void init()
  {
    // Get handle to broker FIRST
    mBroker = new ConfigBrokerProxy();

    // Create a context
    mContext = new ConfigContext();
    mContext.add(mFileSourceName);
  }

  private String mConfigFilePath = new String();
  private JPanel jPanel3 = new JPanel();
  private JPanel jPanel2 = new JPanel();
  private JPanel directionsPanel = new JPanel();
  private JLabel jLabel2 = new JLabel();
  private BorderLayout borderLayout1 = new BorderLayout();
  private JLabel lblTitle = new JLabel();
  private JLabel lblDirections = new JLabel();

  public String getConfigFilePath()
  {
    return(mConfigFilePath);
  }
  public void setConfigFilePath(String path)
  {
    mConfigFilePath = path;
  }


  public void copySimFilesToContext()
  {
    try{
      // Set the paths to the known config files.
      String base_path = this.mConfigFilePath + "/sim.base.jconf";
      String wand_path = this.mConfigFilePath + "/sim.wand.mixin.jconf";

      // Expand in the cas of enviroment variables
      EnvironmentService env_service = new EnvironmentServiceProxy();
      base_path = env_service.expandEnvVars(base_path);
      wand_path = env_service.expandEnvVars(wand_path);

      // Load sim.base.jconf
      FileDataSource base_filesource = FileDataSource.open(base_path, mBroker.getRepository());
      mBroker.add("sim.base.jconf", base_filesource);

      // Load sim.wand.minxin.jconf
      FileDataSource wand_filesource = FileDataSource.open(wand_path, mBroker.getRepository());
      mBroker.add("sim.wand.mixin.jconf", wand_filesource);

      // Create a new context and add both file sources to it
      ConfigContext ctx = new ConfigContext();
      ctx.add("sim.base.jconf");
      ctx.add("sim.wand.mixin.jconf");

      // Get all elements from the two files
      java.util.List temp_list = mBroker.getElements(ctx);

      for(int i = 0 ; i < temp_list.size() ; i++)
      {
        ConfigElement elt = (ConfigElement)temp_list.get(i);
        if(elt.getDefinition().getToken().equals("display_window"))
        {
          mDisplayElementList.add(elt);
        }
        else if(elt.getDefinition().getToken().equals("display_system"))
        {
          mDisplaySystemElementList.add(elt);
        }
        else
        {
          replaceAllConfigFilesNamed(mContext, elt);
          //mBroker.add(mContext, new ConfigChunk(chunk));
        }
      }
      mBroker.remove("sim.base.jconf");
      mBroker.remove("sim.wand.mixin.jconf");
    }
    catch(java.io.IOException exp)
    {
      exp.printStackTrace();
    }
  }
  public void createDisplayChunks()
  {
    //Add Simulator Display Systems and Display Windows to each Node
    java.util.List chunks_list = mBroker.getElements(mContext);
    java.util.List matches = ConfigUtilities.getElementsWithDefinition(chunks_list, "machine_specific");
    for(int i = 0; i < matches.size() ; i++)
    {
      ConfigElement node = (ConfigElement)matches.get(i);

      // Remove all old display_systems
      int num_systems = node.getPropertyValueCount("display_system");
      for (int n=0;n<num_systems;n++)
      {
        node.removeProperty("display_system",n);
      }
      // Remove all old display_windows
      int num_displays = node.getPropertyValueCount("display_windows");
      for (int n=0;n<num_displays;n++)
      {
        node.removeProperty("display_windows",n);
      }
      // Add every display_system chunk
      for(int j = 0; j < mDisplaySystemElementList.size() ; j++)
      {
        ConfigElement display_sys_elt = (ConfigElement)mDisplaySystemElementList.get(j);
        node.setProperty("display_system",j,display_sys_elt);
      }
      // Add every display_window chunk
      for(int j = 0; j < mDisplayElementList.size() ; j++)
      {
        ConfigElement display_elt = (ConfigElement)mDisplayElementList.get(j);
        node.setProperty("display_windows",j,display_elt);
      }
    }
  }

  private void replaceAllConfigFilesNamed(ConfigContext ctx, ConfigElement elt)
  {
    ConfigBroker broker = new ConfigBrokerProxy();

    java.util.List old_list =
        ConfigUtilities.getElementsWithName(mBroker.getElements(ctx), elt.getName());

    for(int i=0;i<old_list.size();i++)
    {
      broker.remove(ctx,(ConfigElement)old_list.get(i));
    }

    broker.add(mContext, elt);
  }
  public boolean saveFile()
  {
    DataSource mFileSource = mBroker.get(mFileSourceName);

    try{
      mFileSource.commit();
    }
    catch(java.io.IOException exp)
    {
      exp.printStackTrace();
    }
//    mContext.remove("ClusterFileSource");
    return(true);
  }

  public void update()
  {
    lstHeadNodes.removeAllElements();
    lstWandNodes.removeAllElements();
    lstCameraNodes.removeAllElements();

    java.util.List chunks_list = mBroker.getElements(mContext);
    java.util.List matches = ConfigUtilities.getElementsWithDefinition(chunks_list, "machine_specific");
    for(int i = 0; i < matches.size() ; i++)
    {
      lstHeadNodes.addElement(((ConfigElement)matches.get(i)).getName());
      lstWandNodes.addElement(((ConfigElement)matches.get(i)).getName());
      lstCameraNodes.addElement(((ConfigElement)matches.get(i)).getName());
    }

  }
  void updateSimDevices(ActionEvent e)
  {
    JComboBox temp_cb = (JComboBox)e.getSource();
    String cb_name = temp_cb.getName();
    System.out.println(cb_name);

    if(e.getActionCommand().equals("headChanged"))
    {
      java.util.List chunk_list = ConfigUtilities.getElementsWithName(mBroker.getElements(mContext),"Head Keyboard");
      if (chunk_list.size() == 1)
      {
        String device_host = (String)cbHeadKeyboard.getSelectedItem();
        ((ConfigElement)chunk_list.get(0)).setProperty("device_host",0,device_host);
      }
      else
      {
        System.out.println("ERROR: You have more than two chunks named, " + cb_name
                           + " in the current configureation");
      }
    }
    else if(e.getActionCommand().equals("cameraChanged"))
    {
      java.util.List chunk_list = ConfigUtilities.getElementsWithName(mBroker.getElements(mContext),"Sim View Cameras Control");
      if (chunk_list.size() == 1)
      {
        String device_host = (String)cbSimCameraKeyboard.getSelectedItem();
        ((ConfigElement)chunk_list.get(0)).setProperty("device_host",0,device_host);
      }
      else
      {
        System.out.println("ERROR: You have more than two chunks named, " + cb_name
                           + " in the current configureation");
      }
    }
    else if(e.getActionCommand().equals("wandChanged"))
    {
      java.util.List chunk_list = ConfigUtilities.getElementsWithName(mBroker.getElements(mContext),"Wand Keyboard");
      if (chunk_list.size() == 1)
      {
        String device_host = (String)cbWandKeyboard.getSelectedItem();
        ((ConfigElement)chunk_list.get(0)).setProperty("device_host",0,device_host);
      }
      else
      {
        System.out.println("ERROR: You have more than two chunks named, " + cb_name
                           + " in the current configureation");
      }
    }
  }
}
