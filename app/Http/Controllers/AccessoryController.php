<?php

namespace App\Http\Controllers;

use App\Models\Accessory;
use App\Models\AccessoryType;
use Illuminate\Http\Request;
use Inertia\Inertia;

class AccessoryController extends Controller
{
    /**
     * Shows the Accessories list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Accessories page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $accessories = Accessory::query();

        if ( $search != "" )
        {
            $accessories->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $accessories->with('accessory_type')->orderby($field, $order);
        }

        return Inertia::render('Accessories', [
            'accessories' => $accessories->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            'accessorytypes' => AccessoryType::all(),
            
            // Send to the client an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }
    
    /**
     * Creates a new Accessory
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the Accessory
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Accessories page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('accessories')->with('error', 'Creazione accessorio non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the Accessory
        $validated = $request->validate([
            'name' => 'required',
            'accessory_type_id' => 'required|exists:accessory_types,id',
        ]);

        // This is executed only if the validation succeedes
        if ( ! Accessory::create($validated) )
        {
            return redirect()->route('accessories')->with('error', 'Creazione accessorio non riuscita.');
        }

        return redirect()->route('accessories')->with('success', 'Accessorio creato con successo.');
    }

    /**
     * Updates an Accessory given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the Accessory
     * @param int $id id of the Accessory to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Accessories page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('accessories')->with('error', 'Accessorio non trovato.');

        $request->validate([
            'name' => 'required',
            'accessory_type_id' => 'required|exists:accessory_types,id',
        ]);

        $accessory = Accessory::find($id);

        if ( ! $accessory )
        {
            return redirect()->route('accessories')->with('error', 'Accessorio non trovato.');
        }

        $accessory->name = $request->input('name');
        $accessory->accessory_type_id = $request->input('accessory_type_id');

        if ( ! $accessory->save() )
            return redirect()->route('accessories')->with('error', 'Modifica accessorio non riuscita.');
            
        return redirect()->route('accessories')->with('success', 'Accessorio modificato con successo.');
    }

    /**
     * Deletes an Accessory given its id
     * 
     * @param int $id id of the Accessory to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Accessories page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('accessories')->with('error', 'Accessorio non trovato.');

        $accessory = Accessory::find($id);

        if ( ! $accessory )
        {
            return redirect()->route('accessories')->with('error', 'Accessorio non trovato.');
        }

        if ( ! $accessory->delete() )
            return redirect()->route('accessories')->with('error', 'Eliminazione accessorio non riuscita.');
            
        return redirect()->route('accessories')->with('success', 'Accessorio eliminato con successo.');
    }
    
    /**
     * Deletes a set of Accessories whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the Accessories to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the Accessories page
     */
    public function multiDelete(Request $request)
    {
        if ( Accessory::destroy(collect($request->input('ids'))) )
            return redirect()->route('accessories')->with('success', 'Accessori eliminati con successo.');

        return redirect()->route('accessories')->with('error', 'Eliminazione accessori non riuscita.');
    }
}
